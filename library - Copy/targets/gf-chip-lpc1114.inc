#ifndef LPC1114_H
#define LPC1114_H

// Memory Mapped Peripheral Control Registers must be accessed as volatile to avoid compiler optimization
#define  MMIO(addr) 					(*(volatile int*) addr)

// Peripheral RESET Register SCB) Registers
#define PRESETCTRL						MMIO(0x40048004)
#define PRESETCTRL_I2C_BIT				1

//  SYSCON Registers (SCB)
#define SYSAHBCLKCTRL  					MMIO(0x40048080)		// System Advanced High-performance Bus Clock Control
#define SYSAHBCLKCTRL_IOCON_BIT 		16
#define SYSAHBCLKCTRL_I2C_BIT 			5
#define SYSAHBCLKCTRL_UART_BIT			12						// Gates system clock to UART

#define UARTCLKDIV						MMIO(0x40048098)		// SYSCON UART Clock Devider Register (UART_CLK from MAIN_CLK)

// IOCON - I2C 
#define IOCON_PIO0_4					MMIO(0x40044030)
#define IOCON_PIO0_5					MMIO(0x40044034)

// IOCON - UART 
#define IOCON_PIO1_6					MMIO(0x400440A4)		// RX pin
#define IOCON_PIO1_7					MMIO(0x400440A8)		// TX pin

// IOCON - GPIO0
#define IOCON_PIO0_8   					MMIO(0x40044060)

// IOCON - GPIO1
#define IOCON_PIO1_8   					MMIO(0x40044014)
#define IOCON_PIO1_9   					MMIO(0x40044038)

// GPIO1 - Registers
#define GPIO1DATA      					MMIO(0x50013FFC)
#define GPIO1DIR      					MMIO(0x50018000)

#define GPIO1_ISENSE					MMIO(0x50018004)
#define GPIO1_IEDGES					MMIO(0x50018008)
#define GPIO1_IEVENT					MMIO(0x5001800C)
#define GPIO1_IENABLE					MMIO(0x50018010)
#define GPIO1_IRAWSTAT					MMIO(0x50018014)
#define GPIO1_IMASKSTAT					MMIO(0x50018018)
#define GPIO1_ICLR						MMIO(0x5001801C)

// SysTick Registers (WORD ADDR)
#define SYSTICK_CSR						MMIO(0xE000E010) 
#define SYSTICK_RVR						MMIO(0xE000E014) 
#define SYSTICK_CVR						MMIO(0xE000E018) 

// SYSTICK_CLKSOURCE : Use CPU (core)'s clock
// SYSTICK_TICKINT 	 : SysTick interrupt is activated on NVIC
// SYSTICK_ENABLE 	 : Enable SysTick
#define SYSTICK_CLKSOURCE_BIT 			2
#define SYSTICK_TICKINT_BIT  			1
#define SYSTICK_ENABLE_BIT 				0

// NVIC Registers (WORD ADDR)
#define NVIC_SETENA						MMIO(0xE000E100)
#define NVIC_CLRENA						MMIO(0xE000E180)

// NVIC IRQs
#define NVIC_I2C_BIT					15
#define NVIC_UART_BIT					21
#define NVIC_GPIO1_BIT					30

// I2C Peripheral REgisters
#define I2C_CTRL_SET					MMIO(0x40000000)
#define I2C_STATUS						MMIO(0x40000004)
#define I2C_DATA						MMIO(0x40000008)
#define I2C_CLK_H						MMIO(0x40000010)
#define I2C_CLK_L						MMIO(0x40000014)
#define I2C_CTRL_CLR					MMIO(0x40000018)

#define I2C_CTRL_AA_BIT					2
#define I2C_CTRL_SI_BIT					3
#define I2C_CTRL_STO_BIT				4
#define I2C_CTRL_STA_BIT				5
#define I2C_CTRL_I2EN_BIT				6


/**********************    UART   *********************/

// Standard UART 16550A Registers
#define UART_RBR						MMIO(0x40008000)		// RX Buffer Register
#define UART_THR						MMIO(0x40008000)		// TX Holding Register
#define UART_DLL						MMIO(0x40008000)		// Divisor Latch LSB
#define UART_DLM						MMIO(0x40008004)		// Divisor Latch MSB
#define UART_IER						MMIO(0x40008004)		// Interrupt Enable Register
#define UART_IIR						MMIO(0x40008008)		// Interrupt Identification Register
#define UART_FCR						MMIO(0x40008008)		// FIFO Control Register
#define UART_LCR						MMIO(0x4000800C)		// Line Control Register
#define UART_MCR						MMIO(0x40008010)		// Modem Control Register
#define UART_LSR						MMIO(0x40008014)		// Line Status Register
#define UART_MSR						MMIO(0x40008018)		// Modem Status Register
#define UART_SCR						MMIO(0x4000801C)		// Scratch Register (NOT of any particular use)

// LPC1114 Specific UART Registers
#define UART_ACR						MMIO(0x40008020)		// Auto-Baud Control Register
#define UART_FDR						MMIO(0x40008028)		// Fractional Devisor Register
#define UART_TER						MMIO(0x40008030)		// TX Enable Register (Enabled by default)

// UART Interrupt Enable Register (IER) Bits
#define UART_IER_RDA_BIT				0						// RX Data Available Interrupt
#define UART_IER_THRE_BIT				1						// TX Holding Register Empty Interrupt
#define UART_IER_LSR_BIT				2						// RX Line Status Register Interrupt (Overrun, Parity Error, Frame Error and Break detected)
#define UART_IER_MSR_BIT				3						// Modem Status Register Interrupt
#define UART_IER_CTS_BIT				7						// CTS Interrupt (only effective in AUTO_CTS mode)
#define UART_IER_ABEO_BIT				8						// End of Auto-Baud Interrupt	(LPC1114 specific)
#define UART_IER_ABTO_BIT				9						// Auto-Baud Timeout Interrupt	(LPC1114 specific)

// UART Interrupt Identification Register (IIR) Bits
#define UART_IIR_INT_BIT				0						// 0: Interrupt pending, 1: No interrupt pending
#define UART_IIR_INTID_BIT				1						// Interrupt Identification
#define UART_IIR_INTID_MASK				0b00001110
#define UART_IIR_ABEO_BIT				8						// End of Auto-Baud  (LPC1114 specific)
#define UART_IIR_ABTO_BIT				9						// Auto-Baud Timeout (LPC1114 specific)

// UART Interrupt Identification Values
#define UART_IIR_INTID_LSR				0x03					// All LSR events except RDA (RX Data Available)
#define UART_IIR_INTID_RDA				0x02					// RX Data Available due to RX FIFO threshold
#define UART_IIR_INTID_CTO				0x06					// Character Time-out due to half-full RX FIFO
#define UART_IIR_INTID_THRE				0x01					// TX Holding Register or TX FIFO is empty (with preconditions)
#define UART_IIR_INTID_MSR				0x00					// State change on Modem Input pins (DCD, DSR and CTS)

// UART FIFO Control Register (FCR) Bits
#define UART_FCR_FIFO_ENABLE_BIT		0
#define UART_FCR_RXFIFO_RESET_BIT		1						// 1: Clear RX FIFO, 0: FIFO unaffected
#define UART_FCR_TXFIFO_RESET_BIT		2						// 1: Clear TX FIFO, 0: FIFO unaffected
#define UART_FCR_RXTRIGGER_BIT			6
#define UART_FCR_RXTRIGGER_MASK			0b11000000

// UART RX FIFO Trigger Level Values 
#define UART_FCR_RXTRIGGER_01			0x00
#define UART_FCR_RXTRIGGER_04			0x01
#define UART_FCR_RXTRIGGER_08			0x02
#define UART_FCR_RXTRIGGER_14			0x03

// UART Line Control Register (LCR) Bits
#define UART_LCR_WORDLENGTH_BIT			0						// Word length control bits
#define UART_LCR_WORDLENGTH_MASK		0b00000011				
#define UART_LCR_WORDLENGTH_05			0x00
#define UART_LCR_WORDLENGTH_06			0x01
#define UART_LCR_WORDLENGTH_07			0x02
#define UART_LCR_WORDLENGTH_08			0x03

#define UART_LCR_STOP_BIT				2
#define UART_LCR_PARITYENABLE_BIT		3

#define UART_LCR_PARITY_BIT				4
#define UART_LCR_PARITY_MASK			0x00110000
#define UART_LCR_PARITY_ODD				0x00
#define UART_LCR_PARITY_EVEN			0x01
#define UART_LCR_PARITY_HIGH			0x02					// Parity bit will always be 1
#define UART_LCR_PARITY_LOW				0x03					// Parity bit will always be 0

#define UART_LCR_BREAK_BIT				6						// 1: Send BREAK signal by pulling TX pin low, 0: Disable BREAK
#define UART_LCR_DLAB_BIT				7						// 0: Disable access to Divisor Latch Registers, 1: Enable accesst to Divisor Latch Registers

// UART Modem Control Register (MCR) Bits
#define UART_MCR_DTR_BIT				0
#define UART_MCR_RTS_BIT				1
#define UART_MCR_LOOPBACK_BIT			4
#define UART_MCR_AUTORTS_BIT			6
#define UART_MCR_AUTOCTS_BIT			7

// UART Line Status Register (LSR) bits
#define UART_LSR_RDA_BIT				0						// RX Data Available
#define UART_LSR_OVERRUN_ERROR_BIT		1
#define UART_LSR_PARITY_ERROR_BIT		2
#define UART_LSR_FRAME_ERROR_BIT		3
#define UART_LSR_BREAK_BIT				4						// BREAK detected on RX pin
#define UART_LSR_THRE_BIT				5						// THR Empty
#define UART_LSR_TXE_BIT				6						// THR + TSR Empty
#define UART_LSR_RXFIFO_ERROR_BIT		7						// ERROR in RX FIFO (may be not the top most byte)

// UART Modem Status Register (MSR) bit
#define UART_MSR_CTS_CHANGE_BIT			0
#define UART_MSR_DSR_CHANGE_BIT			1
#define UART_MSR_RI_CHANGE_BIT			2						// @ trailing edge of Ring Indicator 
#define UART_MSR_DCD_CHANGE_BIT			3
#define UART_MSR_CTS_BIT				4
#define UART_MSR_DSR_BIT				5
#define UART_MSR_RI_BIT					6
#define UART_MSR_DCD_BIT				7

/**********************    End of UART   *****************/


/**********************      SYSPLL      *****************/

#define SYSPLLCTRL						MMIO(0x40048008)
#define SYSPLLCTRL_MDIV_BIT				0

#define SYSPLLCTRL_PDIV_BIT 			5
#define SYSPLLCTRL_PDIV_MASK			0b01100000
#define SYSPLLCTRL_PDIV_01				0x0
#define SYSPLLCTRL_PDIV_02				0x1
#define SYSPLLCTRL_PDIV_04				0x2
#define SYSPLLCTRL_PDIV_08				0x3

#define SYSPLLSTAT						MMIO(0x4004800C)
#define SYSPLLSTAT_LOCKED				0x1

#define MAINCLKSEL						MMIO(0x40048070)
#define MAINCLKSEL_IRC					0x0
#define MAINCLKSEL_PLLIN				0x1
#define MAINCLKSEL_WDT					0x2
#define MAINCLKSEL_PLLOUT				0x3

#define MAINCLKUEN						MMIO(0x40048074)
#define MAINCLKUEN_OFF					0x0
#define MAINCLKUEN_ON					0x1

#define PDCONFIG 						MMIO(0x40048238)
#define PDCONFIG_SYSPLL_BIT				7
#define PDCONFIG_SYSPLL_ON				0x0
#define PDCONFIG_SYSPLL_OFF				0x1

#define SYSPLLCLKSEL					MMIO(0x40048040)
#define SYSPLLCLKSEL_IRC				0x0
#define SYSPLLCLKSEL_OSC				0x1

/**********************   End of SYSPLL  *****************/

#endif /* LPC1114_H */