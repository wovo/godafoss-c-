
namespace demo {

template< typename w, typename tt >
void random_lines(){
   w::init();
   for(;;){
      w::clear( white );
      for( int i = 0; i < 20; ++i ){
         auto start = w::origin + random_xy( w::size );
         auto end = w::origin + random_xy( w::size );
         auto ink = random_color< typename w::color_t >();
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
void random_lines2(){
   w::init();
   for(;;){
      w::clear( white );
      for( int i = 0; i < 20; ++i ){
         auto start = w::origin + random_xy( w::size );
         auto end = w::origin + random_xy( w::size );
// std::cout << start << " " << end << " " << ink << "\n";
         godafoss::line< w >( start, end, black ).write();
         w::flush();
//         using ms = typename tt::ms;
//         using d = template ms< 200 >;
         tt::wait();
      }
   }
}

};
