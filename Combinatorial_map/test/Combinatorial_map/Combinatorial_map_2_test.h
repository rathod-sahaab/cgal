// Copyright (c) 2010-2011 CNRS (France), All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
// Author(s)     : Guillaume Damiand <guillaume.damiand@liris.cnrs.fr>
//
#ifndef CGAL_COMBINATORIAL_MAP_2_TEST
#define CGAL_COMBINATORIAL_MAP_2_TEST 1

#include <CGAL/Combinatorial_map_constructors.h>
#include <CGAL/Combinatorial_map_operations.h>

#include <CGAL/IO/Polyhedron_iostream.h>
#include <iostream>
#include <fstream>

using namespace std;

template<class Map>
void drawCell2 ( Map& amap, typename Map::Dart_handle adart, int aorbit, int mark )
{
  /*    cout << "Orbite " << Map::ORBIT_NAME[aorbit] << " (";
    CGAL::CMap_dart_iterator_basic_of_orbit_2<Map> it2 ( amap, adart, aorbit, mark );
    for ( ;it2.cont(); ++it2 )
    {
        cout << ( *it2 )->vertex()->point() << ", ";
    }
    cout << ")" << flush;*/
}

template<class Map>
void drawAllPoints( Map&amap )
{
  amap.display_characteristics(std::cout);
  /*  for ( typename Map::Dart_range::iterator it=amap.darts().begin();
	 it!=amap.darts().end(); ++it )
    cout << &*it<< ",  ";
    cout<<endl;*/
}

template<class Map>
void test2D()
{
  typedef typename Map::Dart_handle Dart_handle;

    Map map;
    Dart_handle dh, dh2, d1, d2, d3;
    int mark;
    unsigned int nbc, nb2;

    cout<<"Size of dart:"<<sizeof(typename Map::Dart)<<std::endl;
    cout << "***************************** TEST BASIC CREATION 2D:"
         << endl;

    dh = map.create_dart ();
    dh2 = map.create_dart();

    if ( map.is_valid() ) cout << "Map valid." << endl;
    cout << "Nombre de brins : " << map.number_of_darts() << endl;
    map.clear();

    cout << "***************************** TEST BASIC CREATION 2D DONE."
         << endl;

    cout << "***************************** TEST CREATION WITH EMBEDDING 2D:"
         << endl;

    dh = map.create_dart ();
    dh2 = map.create_dart();

    cout << "Parcours all : "; drawAllPoints(map);

    map.template sew<2> ( dh, dh2 );
    cout << "Parcours all : "; drawAllPoints(map);
    cout << endl << endl;

    make_edge ( map );

    dh = make_combinatorial_tetrahedron ( map );
    cout << "Nombre de brins : " << map.number_of_darts() << endl;

    cout << "Parcours de CC : ";
    for ( typename Map::template Dart_of_orbit_range<1,2>::iterator it ( map, dh );
            it.cont();++it )
    {
        if ( it.prev_operation() != CGAL::OP_BETAI &&
                it.prev_operation() != CGAL::OP_BETAI_INV )  cout << "\nNew facet : ";
	//        cout << &*it << ", ";
    }
    cout << endl;

    cout << "Parcours all : "; drawAllPoints(map);

    if ( map.is_valid() ) cout << "Map valid." << endl;

    map.clear();

    cout << "***************************** TEST CREATION WITH EMBEDDING 2D DONE."
         << endl;

    cout << "***************************** TEST SEW 2D:" << endl;

    make_edge ( map );

    d1 = make_combinatorial_polygon ( map,3 );

    d2 = make_combinatorial_polygon ( map,3 );

    cout << "Parcours all : "; drawAllPoints(map);

    map.template sew<2> ( d1, d2 );

    cout << "Parcours all : "; drawAllPoints(map);

    if ( map.is_valid() ) cout << "Map valid." << endl;
    map.clear();

    cout << "***************************** TEST SEW 2D DONE." << endl;

    cout << "***************************** TEST TRIANGULATION_2 2D:" << endl;

    d1 = make_combinatorial_tetrahedron ( map );

    cout << "Parcours all before insert_cell_0_in_cell_2: ";
    drawAllPoints(map);

    // Triangulate the facet between the two triangles
    insert_cell_0_in_cell_2( map, d1 );

    cout << "Parcours all after insert_cell_0_in_cell_2: ";
    drawAllPoints(map);

    if ( map.is_valid() ) cout << "Map valid." << endl;

    map.clear();

    cout << "***************************** TEST TRIANGULATION_2 2D DONE."
         << endl;

    cout << "***************************** TEST ITERATORS 2D:" << endl;

    for ( int i = 0; i < 1000; ++i )
    {
      d1 = make_combinatorial_polygon ( map,3 );
      d2 = make_combinatorial_polygon ( map,3 );
      d3 = make_combinatorial_polygon ( map,3 );
        // Sew the 2 tetrahedra along one facet
        map.template sew<2> ( d1, d2 );
        map.template sew<2> ( d2->beta ( 1 ), d3 );
        map.template sew<2> ( d1->beta ( 1 ), d3->beta ( 1 ) );
    }

    // Two nested iterators
    cout << "Nombre de brins : " << map.number_of_darts() << ", "
         << "Nombre de CC : " << flush;
    mark = map.get_new_mark();
    nbc = 0;
    for ( typename Map::Dart_range::iterator it1=map.darts().begin();
	  it1!=map.darts().end(); ++it1 )
    {
        if ( !map.is_marked ( it1, mark ) )
        {
            ++nbc;
            for ( typename Map::template Dart_of_orbit_range<1,2>::iterator
		    it2 ( map, it1 ); it2.cont(); ++it2 )
            {
                map.mark ( it2, mark );
            }
        }
    }
    cout << nbc << endl;
    cout << "All darts marked? " << map.is_whole_map_marked ( mark )
         << endl;
    map.unmark_all ( mark );

    // Tout les parcours possibles :
    // TODO static foreach
    /*    for ( int i = CGAL::SELF_ORBIT; i <= CGAL::ALL_DARTS_ORBIT; ++i )
    {
        cout << "Parcours orbite " << Map::ORBIT_NAME[i] << " : #cellules=" << flush;
        nbc = 0, nb2 = 0;
	for ( typename Map::Dart_iterator it1=map.darts_begin();it1!=map.darts_end(); ++for )
        {
            ++nb2;
            if ( !map.is_marked ( *it1, mark ) )
            {
                ++nbc;
                CGAL::CMap_dart_iterator_basic_of_orbit_2<Map> it2 ( map, *it1, i, mark );
                for ( ;it2.cont(); ++it2 )
                    {}
            }
	    }
        cout << nbc << "." << ", #brins=" << nb2 << "." << endl
             << "All the darts marked ? " << map.is_whole_map_marked ( mark )
             << endl;
	     map.unmark_all ( mark );
    }*/

    // Iterator stl like
    { 
      nbc = 0; nb2 = 0;
      cout << "Iterator stl like: #cellules=" << flush;
      for ( typename Map::Dart_range::iterator it1= map.darts().begin();
	    it1!=map.darts().end(); ++it1 )
        {
	  ++nb2;
	  if ( !map.is_marked ( it1, mark ) )
            {
                ++nbc;
                for ( typename Map::template Dart_of_orbit_range<2>::iterator
                        it2 ( map.template darts_of_orbit<2>(it1).begin() );
		      it2 != map.template darts_of_orbit<2>(it1).end(); ++it2 )
                {
                    map.mark ( it2, mark );
                }
            }
        }
        cout << nbc << "." << ", #brins=" << nb2 << "." << endl
             << "All the darts marked ? " << map.is_whole_map_marked ( mark ) << endl;
        map.unmark_all ( mark );
    }
    map.free_mark ( mark );
    map.clear();

    cout << "***************************** TEST ITERATORS 2D DONE." << endl;

    cout << "***************************** TEST INCIDENCE ITERATORS 2D:"
         << endl;

    d1 = make_combinatorial_polygon ( map,4 );
    d2 = make_combinatorial_polygon ( map,4 );
    map.template sew<2> ( d1, d2 );
    cout << "Map valid : " << map.is_valid() << endl;

    mark = map.get_new_mark();

    // Un parcours de cellule.
    {
      CGAL::mark_orbit<Map,CGAL::CMap_dart_const_iterator_basic_of_orbit<Map,1> >
	( map, map.first_dart(), mark);

        nbc = 0;
        for ( typename Map::template One_dart_per_incident_cell_range<0,3>::iterator 
		it1 ( map, map.first_dart() ); it1.cont(); ++it1 )
        {
            ++nbc;
        }
        std::cout << "Nombre de sommet de la premiere cc : " << nbc << std::endl;
    }
    map.free_mark ( mark );
    map.clear();

    cout << "***************************** TEST INCIDENCE ITERATORS 2D DONE."
         << endl;

    cout << "***************************** TEST VERTEX REMOVAL 2D:"
         << endl;

    d1 = map.create_dart ();
    map.display_characteristics ( cout ) << ", valid=" << map.is_valid() << endl;
    cout << "remove vertex1: " << flush;
    CGAL::remove_cell<Map,0> ( map, d1 );
    map.display_characteristics (cout)<<", valid=" << map.is_valid() << endl;

    d1 = map.create_dart ();
    map.template sew<1> ( d1, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    cout << "remove vertex2: " << flush;
    CGAL::remove_cell<Map,0> ( map, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    d1 = make_edge ( map );
    d2 = d1->beta ( 2 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    cout << "remove vertex3: " << flush;
    CGAL::remove_cell<Map,0> ( map, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    cout << "remove vertex4: " << flush;
    CGAL::remove_cell<Map,0> ( map, d2 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    d1 = make_edge ( map );
    map.template sew<1> ( d1, d1 );
    map.template sew<1> ( d1->beta ( 2 ), d1->beta ( 2 ) );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    cout << "remove vertex5: " << flush;
    CGAL::remove_cell<Map,0> ( map, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    d1 = make_combinatorial_polygon ( map,3 );
    d2 = d1->beta ( 0 );
    d3 = d1->beta ( 1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    cout << "remove vertex6: " << flush;
    CGAL::remove_cell<Map,0> ( map, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    cout << "remove vertex7: " << flush;
    CGAL::remove_cell<Map,0> ( map, d2 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    cout << "remove vertex8: " << flush;
    CGAL::remove_cell<Map,0> ( map, d3 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    d1 = make_combinatorial_polygon ( map,3 );
    d2 = make_combinatorial_polygon ( map,3 );
    map.template sew<2> ( d1, d2 );
    map.template sew<2> ( d1->beta ( 0 ), d2->beta ( 1 ) );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    cout << "remove vertex9: " << flush;
    CGAL::remove_cell<Map,0> ( map, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    map.clear();

    cout << "***************************** TEST VERTEX REMOVAL 2D DONE."
         << endl;

    cout << "***************************** TEST EDGE REMOVAL 2D:"
         << endl;

    d1 = map.create_dart ();
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    cout << "remove edge1: " << flush;
    CGAL::remove_cell<Map,1> ( map, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    d1 = map.create_dart ();
    map.template sew<1> ( d1, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    cout << "remove edge2: " << flush;
    CGAL::remove_cell<Map,1> ( map, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    d1 = make_edge ( map );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    cout << "remove edge3: " << flush;
    CGAL::remove_cell<Map,1> ( map, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    d1 = make_edge ( map );
    map.template sew<1> ( d1, d1 );
    map.template sew<1> ( d1->beta ( 2 ), d1->beta ( 2 ) );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    cout << "remove edge4: " << flush;
    CGAL::remove_cell<Map,1> ( map, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    d1 = make_edge ( map );
    map.template sew<1> ( d1, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    cout << "remove edge5: " << flush;
    CGAL::remove_cell<Map,1> ( map, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    d1 = make_combinatorial_polygon ( map,3 );
    d2 = d1->beta ( 0 );
    d3 = d1->beta ( 1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    cout << "remove edge6: " << flush;
    CGAL::remove_cell<Map,1> ( map, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    cout << "remove edge7: " << flush;
    CGAL::remove_cell<Map,1> ( map, d2 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    cout << "remove edge8: " << flush;
    CGAL::remove_cell<Map,1> ( map, d3 );
    map.display_characteristics ( cout)<<", valid=" << map.is_valid() << endl;

    d1 = make_combinatorial_polygon ( map,3 );
    d2 = make_combinatorial_polygon ( map,3 );
    map.template sew<2> ( d1, d2 );
    d2 = d1->beta ( 0 );
    d3 = d1->beta ( 1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    cout << "remove edge9: " << flush;
    CGAL::remove_cell<Map,1> ( map, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    cout << "remove edge10: " << flush;
    CGAL::remove_cell<Map,1> ( map, d2 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    cout << "remove edge11: " << flush;
    CGAL::remove_cell<Map,1> ( map, d3 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    d1 = make_combinatorial_polygon ( map,3 );
    d2 = make_combinatorial_polygon ( map,3 );
    map.template sew<2> ( d1, d2 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    cout << "remove edge12: " << flush;
    CGAL::remove_cell<Map,1> ( map, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    d1 = make_combinatorial_polygon ( map,3 );
    d2 = make_combinatorial_polygon ( map,3 );
    map.template sew<2> ( d1, d2 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    cout << "remove edge13: " << flush;
    CGAL::remove_cell<Map,1> ( map, d1->beta ( 1 ) );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    cout << "remove edge14: " << flush;
    CGAL::remove_cell<Map,1> ( map, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    d1 = make_combinatorial_hexahedron ( map );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    insert_cell_0_in_cell_2 ( map, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;

    std::vector<Dart_handle> V;
    {
      for ( typename Map::template Dart_of_cell_range<0>::iterator it =
	      map.template darts_of_cell<0>( d1 ).begin();
	    it != map.template darts_of_cell<0>( d1 ).end(); ++it )
            V.push_back ( it );
    }

    {
        typedef typename std::vector<Dart_handle>::iterator vector_iterator;
        for ( vector_iterator it = V.begin(); it != V.end(); ++it )
        {
            cout << "remove edge15: " << flush;
            CGAL::remove_cell<Map,1> ( map, *it );
            map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
        }
    }

    map.clear();

    cout << "***************************** TEST EDGE REMOVAL 2D DONE."
         << endl;

    cout << "***************************** TEST INSERT EDGE 2D:"
         << endl;

    /*    d1 = map.create_dart ( Point ( 0, 0, 0 ) );
    d2 = map.create_dart ( Point ( 1, 0, 0 ) );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    cout << "insert edge1: " << flush;
    insert_edge ( map, d1, d2 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    map.clear();*/

    /*    d1 = make_edge ( map, Point ( 0, 0, 0 ), Point ( 0, 0, 1 ) );
    map.template sew<1> ( d1, d1 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    cout << "insert edge2: " << flush;
    insert_edge ( map, d1, d1->beta ( 2 ) );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    map.clear();*/

    d1 = make_combinatorial_polygon ( map,4 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    cout << "insert edge3: " << flush;
    insert_cell_1_in_cell_2 ( map, d1, d1->beta(1)->beta(1) );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    map.clear();

    d1 = make_combinatorial_polygon ( map,4 );
    d2 = make_combinatorial_polygon ( map,4 );
    map.template sew<2> ( d1, d2 );
    map.display_characteristics ( cout ) << ", valid=" << map.is_valid()
				  << endl;
    cout << "insert edge4: "
         << flush;
    insert_cell_1_in_cell_2 ( map, d1, d1->beta(1)->beta(1) );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    map.clear();

    /*    d1 = make_quadrilateral ( map, Point ( 0, 0, 0 ), Point ( 0, 0, 1 ),
                              Point ( 1, 0, 1 ), Point ( 1, 0, 0 ) );
    d2 = make_quadrilateral ( map, Point ( 0, 0, 0 ), Point ( 0, 0, 1 ),
                              Point ( 1, 0, 1 ), Point ( 1, 0, 0 ) );
    map.template sew<2> ( d1, d2 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    cout << "insert edge5: " << flush;
    insert_cell_1_in_cell_2 ( map, d1, d2 );
    map.display_characteristics (  cout ) << ", valid=" << map.is_valid() << endl;
    map.clear();*/

    cout << "***************************** TEST INSERT EDGE 2D DONE."
         << endl;
}

#endif // CGAL_COMBINATORIAL_MAP_2_TEST
