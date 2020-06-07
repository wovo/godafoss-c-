template< typename T = int >
class xy { }; 

template< xy _size >
struct window_root { };

template< typename minion >
struct flip_horizontally : window_root< minion::size >{ };
