// location == template, or location == concept ?


template< typename T, unsigned int N >
class location {
private:
   T [ N ];
public:
   ...
};

template< typename T >
concept bool color = requires( 
   
);

// needs currying to get to pixels of one type but different size
template< typename Location, typename Color >
class pixels {
};