

/// absolute version of an arithmetic value type
///
/// For a value type that denotes a ratio scale value (a value
/// for which addition yields a value on the same scale), the
/// torsor of that type is the corresponding interval scale 
/// (anchored) type.
///
/// Examples of ratio scales and their corresponding anchored 
/// interval scales are:
/// temperature difference - absolute temperature
/// distance vector - location
/// duration - moment in time
///
/// In a unit system like SI a torsor and its ratio type have the 
/// same unit. 
/// But just like adding two values that have different SI units 
/// makes no sense, adding two torsor values makes no sense.
/// This torsor class template uses the type system to block
/// such meaningless operations at compile time. 
/// It is designed to have zero runtime overhead.
///
/// 
template< typename T >
class torsor {
private:

   T value;
   
public:

   torsor():value( 0 ){}
   
   template< typename U >
   constexpr torsor operator+( const U & right ) const {
      return torsor( value + right );      
   }
   
   template< typename U >
   constexpr torsor operator-( const U & right ) const {
      return torsor( value - right );      
   }
   
   template< typename U >
   constexpr auto operator-( const torsor< U > & right ) const {
      return value - right;      
   }
   

};