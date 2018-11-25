template< typename T >
concept bool timer = requires( 
   T::is_timer,
   input< T >
);

constexpr T::value_type wraparound_gap;
constexpr T::value_type maximum;
constexpr T::some_type  frequency;