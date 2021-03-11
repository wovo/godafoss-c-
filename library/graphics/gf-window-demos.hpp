
namespace demo {

template< typename w, typename tt, uint64_t size = 3, xy<> speed = { 1, 1 } >
struct bouncing_square {

   using resources = use_list< w, tt >;

   static void GODAFOSS_NO_RETURN run(){
      auto location = typename w::offset_t( 0, 0 );
      auto delta  = typename w::offset_t( speed );
      for(;;){
         location += delta;
         if( ( location.x <= 0 ) || ( location.x + size + 1 >= w::size.x ) ){ delta.x *= -1; }
         if( ( location.y <= 0 ) || ( location.y + size + 1 >= w::size.y ) ){ delta.y *= -1; }
         w::clear( white );
         rectangle< w >( w::origin + location, typename w::offset_t( size, size ), black, black ).write();
         w::flush();
         tt::wait();
      }
   }
};

template< typename w, typename tt >
void random_lines(){
   w::init();
   tt::init();
   for(;;){
      w::clear( white );
      for( int i = 0; i < 20; ++i ){
         auto start = w::origin + random_xy( w::size );
         auto end = w::origin + random_xy( w::size );
         auto ink = typename w::color_t::random();
// std::cout << start << " " << end << " " << ink << "\n";
         godafoss::line< w >( start, end, ink ).write();
         w::flush();
//         using ms = typename tt::ms;
//         using d = template ms< 200 >;
         tt::wait();
      }
   }
}

template< typename w, typename tt >
struct random_lines2 {

   using resources = use_list< w, tt >;

   static void GODAFOSS_NO_RETURN run(){
      for(;;){
         w::clear( white );
         for( int i = 0; i < 20; ++i ){
            auto start = w::origin + random_xy( w::size );
            auto end = w::origin + random_xy( w::size );
            godafoss::line< w >( start, end, black ).write();
            w::flush();
            tt::wait();
         }
      }
   }
};

};
