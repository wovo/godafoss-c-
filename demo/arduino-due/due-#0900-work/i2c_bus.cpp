

    void i2c_bus_c::pin_init() {
        auto config_pin = [](uint32_t pin, Pio *pio) {
            pio->PIO_ABSR &= (~pin & PIOA->PIO_ABSR);
            pio->PIO_PDR = pin;
            pio->PIO_IDR = pin;
            pio->PIO_PUER = pin;
        };

        if (_selected == TWI0) {
            config_pin(PIO_PA18A_TWCK0, PIOA);
            config_pin(PIO_PA17A_TWD0, PIOA);
        } else if (_selected == TWI1) {
            config_pin(PIO_PB13A_TWCK1, PIOB);
            config_pin(PIO_PB12A_TWD1, PIOB);
        }
    }

    void i2c_bus_c::clock_init() {
        // enable the peripheral clock for twi interface
        if (_selected == TWI0) {
            if ((PMC->PMC_PCSR0 & (1 << ID_TWI0)) != (1 << ID_TWI0)) {
                PMC->PMC_PCER0 |= 1 << ID_TWI0;
            }
        } else if (_selected == TWI1) {
            if ((PMC->PMC_PCSR0 & (1 << ID_TWI1)) != (1 << ID_TWI1)) {
                PMC->PMC_PCER0 |= 1 << ID_TWI1;
            }
        }

        // Disable PDC channel
        _selected->TWI_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

        // TWI Slave Mode Enabled
        _selected->TWI_CR = TWI_CR_SVEN;

        // TWI disable and sw reset
        _selected->TWI_CR = TWI_CR_SWRST;
        _selected->TWI_RHR;

        // Wait for at least 10ms
        timing::ms< 10 >::wait();

        // TWI Slave mode disabled, Twi master mode disabled
        _selected->TWI_CR = TWI_CR_SVDIS | TWI_CR_MSDIS;

        // Enable master mode
        _selected->TWI_CR = TWI_CR_MSEN;

        // this sets the twi frequency
        constexpr uint32_t masterClock = 84000000; ///< Master clock rate 84MHz
        uint32_t ckdiv = 0;                        ///< Clock divider
        uint32_t cLHDiv = 0; ///< Clock low and high divider
        cLHDiv = masterClock / (SPEED * 2) - 4;

        while ((cLHDiv > 0xFF) &&
               (ckdiv <
                7)) { ///< cldiv must fit in 8 bits, ckdiv must fit in 3 bits
            ckdiv++;  ///< Increase clock devider
            cLHDiv /= 2;
        }
        _selected->TWI_CWGR = TWI_CWGR_CLDIV(cLHDiv) | TWI_CWGR_CHDIV(cLHDiv) |
                              TWI_CWGR_CKDIV(ckdiv);
    }

    void i2c_bus_c::init() {
        static bool init_done = false;
        if (init_done) {
            return;
        }
        if (_selected == nullptr) {
            for(;;){}
        }
        pin_init();
        clock_init();
        init_done = true;
    }

    void i2c_bus_c::write_byte(const uint8_t data) {
        _selected->TWI_THR = data;
    }

    uint8_t i2c_bus_c::read_byte() {
        return _selected->TWI_RHR;
    }

    i2c_bus_c::i2c_bus_c(const interface &selected_interface,
                         const uint32_t SPEED)
        : SPEED(SPEED) {
        if (selected_interface == interface::interface_0) {
            _selected = TWI0;
        } else if (selected_interface == interface::interface_1) {
            _selected = TWI1;
        }
        init();
    }

    void i2c_bus_c::write(const uint_fast8_t address, const uint8_t data[],
                          const size_t n, const uint32_t internal_address,
                          const uint8_t internal_address_size) {
        _selected->TWI_MMR = 0; ///< Reset master mode register
        _selected->TWI_MMR =
            0 << 12 | address << 16 |
            internal_address_size
                << 8; ///< Set write, address and internal_address_size
        _selected->TWI_IADR = (0x00FFFFFF & internal_address);

        uint32_t status = 0;
        uint32_t timeout = 0;
        for (size_t i = 0; i < n; i++) {
            status = _selected->TWI_SR;
            if (status & TWI_SR_NACK) {
                for(;;){}
            }
            while (1) {
                timeout++;
                status = _selected->TWI_SR;
                if (status & TWI_SR_TXRDY) {
                    write_byte(data[i]);
                    timeout = 0;
                    break;
                }
                if (timeout >= timeout_counter) {
                    return;
                }
            }
        }

        _selected->TWI_CR = TWI_CR_STOP;
        // Wait until the I2C bus is released.
        while (!(_selected->TWI_SR & TWI_SR_TXCOMP)) {
        };
    }

    void i2c_bus_c::read(const uint8_t address, uint8_t *data, const uint32_t n,
                         const uint32_t internal_address,
                         const uint8_t internal_address_size) {

        _selected->TWI_MMR = 0; ///< Reset master mode register
        _selected->TWI_MMR =
            1 << 12 | address << 16 |
            internal_address_size
                << 8; ///< Set write, address and internal_address_size
        _selected->TWI_IADR = (0xFFFFFF & internal_address);

        uint32_t status = 0; ///< Variable for holding status register
        uint32_t count = n;
        uint8_t stopTransaction =
            0; ///< Byte to indicate that transaction should be stopped

        if (count == 1) { ///< When only one byte needs to be read, transaction
                          ///< should be started and stopped at once.
            _selected->TWI_CR = TWI_CR_START | TWI_CR_STOP;
            stopTransaction = 1;
        } else {
            _selected->TWI_CR = TWI_CR_START;
        }

        uint32_t timeout = 0;
        while (count > 0) {
            timeout++;
            status = _selected->TWI_SR;

            if (count == 1 && !stopTransaction) {
                _selected->TWI_CR = TWI_CR_STOP;
                stopTransaction = 1;
            }

            if (!(status & TWI_SR_RXRDY)) {
                if (timeout >= timeout_counter) {
                    return;
                }
                continue;
            }
            data[n - count] = read_byte();
            timeout = 0;
            count--;
        }
        // Wait until the I2C bus is released.
        while (!(_selected->TWI_SR & TWI_SR_TXCOMP)) {
        };
    }

    void i2c_bus_c::change_clock_speed(const uint32_t SPEED) {
        this->SPEED = SPEED;
        clock_init();
    }
