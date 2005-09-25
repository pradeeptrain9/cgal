// Copyright (c) 2005  Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $Source$
// $Revision$ $Date$
// $Name$
//
// Author(s)     : Ron Wein          <wein@post.tau.ac.il>
#ifndef CGAL_ARR_ACCESSOR_H
#define CGAL_ARR_ACCESSOR_H

/*! \file
 * Definition of the Arr_accessor<Arrangement> class.
 */

CGAL_BEGIN_NAMESPACE

/*! \class
 * A class that provides access to some of the internal arrangement operations.
 * Used mostly by the global insertion functions and by the sweep-line visitors
 * for utilizing topological and geometrical information available during the
 * algorithms they perform.
 * The Arrangement parameter corresponds to an arrangement instantiation.
 */
template <class Arrangement_>
class Arr_accessor
{
public:

  typedef Arrangement_                                  Arrangement_2;
  typedef Arr_accessor<Arrangement_2>                   Self;

  typedef typename Arrangement_2::Point_2               Point_2;
  typedef typename Arrangement_2::X_monotone_curve_2    X_monotone_curve_2;

  typedef typename Arrangement_2::Vertex_handle         Vertex_handle;
  typedef typename Arrangement_2::Vertex_const_handle   Vertex_const_handle;
  typedef typename Arrangement_2::Halfedge_handle       Halfedge_handle;
  typedef typename Arrangement_2::Halfedge_const_handle Halfedge_const_handle;
  typedef typename Arrangement_2::Face_handle           Face_handle;
  typedef typename Arrangement_2::Face_const_handle     Face_const_handle;
  typedef typename Arrangement_2::Ccb_halfedge_circulator
                                                        Ccb_halfedge_circulator;

private:

  typedef typename Arrangement_2::DVertex               DVertex;
  typedef typename Arrangement_2::DHalfedge             DHalfedge;
  typedef typename Arrangement_2::DFace                 DFace;

  Arrangement_2  *p_arr;           // The associated arrangement.

public:

  /*! Constructor with an associated arrangement. */
  Arr_accessor (Arrangement_2& arr) :
    p_arr (&arr)
  {}

  /// \name Accessing the notification functions (for the global functions).
  //@{

  /*! Notify that a global operation is about to take place. */
  void notify_before_global_change ()
  {
    p_arr->_notify_before_global_change();
  }

  /*! Notify that a global operation was completed. */
  void notify_after_global_change ()
  {
    p_arr->_notify_after_global_change();
  }
  //@}

  /// \name Local operations and predicated for the arrangement.
  //@{

  /*!
   * Locate the place for the given curve around the given vertex.
   * \param vh A handle for the arrangement vertex.
   * \param cv The given x-monotone curve.
   * \pre v is one of cv's endpoints.
   * \return A handle for a halfedge whose target is v, where cv should be
   *         inserted between this halfedge and the next halfedge around this
   *         vertex (in a clockwise order).
   */
  Halfedge_handle locate_around_vertex (Vertex_handle vh,
                                        const X_monotone_curve_2& cv) const
  {
    DHalfedge*  he = p_arr->_locate_around_vertex (p_arr->_vertex (vh), cv);

    CGAL_assertion (he != NULL);
    return (p_arr->_handle_for (he));
  }

  /*!
   * Compute the distance (in halfedges) between two halfedges.
   * \param e1 A handle for the source halfedge.
   * \param e2 A handle for the destination halfedge.
   * \return In case e1 and e2 belong to the same connected component, the 
   *         function returns number of boundary halfedges between the two 
   *         halfedges. Otherwise, it returns (-1).
   */
  int halfedge_distance (Halfedge_const_handle e1,
                         Halfedge_const_handle e2) const
  {
    return (p_arr->_halfedge_distance (p_arr->_halfedge (e1),
                                       p_arr->_halfedge(e2)));
  }

  /*!
   * Determine whether a given query halfedge lies in the interior of a new
   * face we are about to create, by connecting it with another halfedge
   * using a given x-monotone curve.
   * \param prev1 A handle for the query halfedge.
   * \param prev2 The other halfedge we are about to connect with prev1.
   * \param cv The x-monotone curve we use to connect prev1 and prev2.
   * \pre prev1 and prev2 belong to the same connected component, and by
   *      connecting them using cv we form a new face.
   * \return (true) if prev1 lies in the interior of the face we are about
   *         to create, (false) otherwise - in which case prev2 must lie
   *         inside this new face.
   */
  bool is_inside_new_face (Halfedge_handle prev1,
                           Halfedge_handle prev2,
                           const X_monotone_curve_2& cv) const
  {
    return (p_arr->_is_inside_new_face (p_arr->_halfedge (prev1),
                                        p_arr->_halfedge (prev2),
                                        cv));
  }

  /*!
   * Determine whether a given point lies within the region bounded by
   * a boundary of a connected component.
   * \param p The query point.
   * \param he A handle for a halfedge on the boundary of the connected
   *           component.
   * \return (true) if the point lies within region, (false) otherwise.
   */
  bool point_is_in (const Point_2& p, 
                    Halfedge_const_handle he) const
  {
    return (p_arr->_point_is_in (p, NULL, p_arr->_halfedge (he)));
  }

  /*!
   * Check whether the given halfedge lies on the outer boundary of the given
   * face.
   * \param f A handle for the given face.
   * \param e The given halfedge.
   * \return (true) in case e lies on the outer boundary of f;
   *         (false) otherwise.
   */
  bool is_on_outer_boundary (Face_const_handle f,
                             Halfedge_const_handle e) const
  {
    return (p_arr->_is_on_outer_boundary (p_arr->_face (f),
                                          p_arr->_halfedge (e)) != NULL);
  }

  /*!
   * Check whether the given halfedge lies on the inner boundary of the given
   * face.
   * \param f A handle for the given face.
   * \param e The given halfedge.
   * \return (true) in case e lies on the outer boundary of f;
   *         (false) otherwise.
   */
  bool is_on_inner_boundary (Face_const_handle f,
                             Halfedge_const_handle e) const
  {
    return (p_arr->_is_on_inner_boundary (p_arr->_face (f),
                                          p_arr->_halfedge (e)) != NULL);
  }

  /*!
   * Create a new vertex and associate it with the given point.
   * \param p The point.
   * \return A handle to the newly created vertex.
   */
  Vertex_handle create_vertex (const Point_2& p)
  {
    DVertex* v = p_arr->_create_vertex (p);
    
    CGAL_assertion (v != NULL);
    return (p_arr->_handle_for (v));    
  }
  
  /*!
   * Insert an x-monotone curve into the arrangement, where the end vertices
   * are given by the target points of two given halfedges.
   * The two halfedges should be given such that in case a new face is formed,
   * it will be the incident face of the halfedge directed from the first
   * vertex to the second vertex.
   * \param cv the given curve.
   * \param prev1 The reference halfedge for the first vertex.
   * \param prev2 The reference halfedge for the second vertex.
   * \param new_face Output - whether a new face has been created.
   * \return A handle for one of the halfedges corresponding to the inserted
   *         curve directed from prev1's target to prev2's target.
   *         In case a new face has been created, it is given as the incident
   *         face of this halfedge.
   */
  Halfedge_handle insert_at_vertices_ex (const X_monotone_curve_2& cv,
                                         Halfedge_handle prev1, 
                                         Halfedge_handle prev2,
                                         bool& new_face)
  {
    DHalfedge*  he = p_arr->_insert_at_vertices (cv,
                                                 p_arr->_halfedge (prev1),
                                                 p_arr->_halfedge (prev2),
                                                 new_face);

    CGAL_assertion (he != NULL);
    return (p_arr->_handle_for (he));
  }

   /*!
   * Insert an x-monotone curve into the arrangement, such that one of its
   * endpoints corresponds to a given arrangement vertex, given the exact
   * place for the curve in the circular list around this vertex. The other
   * endpoint corrsponds to a free vertex (a newly created vertex or an
   * isolated vertex).
   * \param cv The given x-monotone curve.
   * \param prev The reference halfedge. We should represent cv as a pair
   *             of edges, one of them should become prev's successor.
   * \param v The free vertex that corresponds to the other endpoint.
   * \return A handle to one of the halfedges corresponding to the inserted
   *         curve, whose target is the vertex v.
   */
  Halfedge_handle insert_from_vertex_ex (const X_monotone_curve_2& cv,
                                         Halfedge_handle prev,
                                         Vertex_handle v)
  {
    DHalfedge*  he = p_arr->_insert_from_vertex (cv,
                                                 p_arr->_halfedge (prev),
                                                 p_arr->_vertex (v));

    CGAL_assertion (he != NULL);
    return (p_arr->_handle_for (he));
  }

  /*!
   * Insert an x-monotone curve into the arrangement, such that both its
   * endpoints correspond to free arrangement vertices (newly created vertices
   * or existing isolated vertices), so a new hole is formed in the face
   * that contains the two vertices.
   * \param cv The given x-monotone curve.
   * \param f The face containing the two end vertices.
   * \param v1 The free vertex that corresponds to the left endpoint of cv.
   * \param v2 The free vertex that corresponds to the right endpoint of cv.
   * \return A handle to one of the halfedges corresponding to the inserted
   *         curve, directed from v1 to v2.
   */
  Halfedge_handle insert_in_face_interior_ex (const X_monotone_curve_2& cv,
                                              Face_handle f,
                                              Vertex_handle v1,
                                              Vertex_handle v2)
  {
    DHalfedge*  he = p_arr->_insert_in_face_interior (cv,
                                                      p_arr->_face (f),
                                                      p_arr->_vertex (v1),
                                                      p_arr->_vertex (v2));

    CGAL_assertion (he != NULL);
    return (p_arr->_handle_for (he));
  
  }

  /*!
   * Insert the given vertex as an isolated vertex inside the given face.
   * \param f The face that should contain the isolated vertex.
   * \param v The isolated vertex.
   */
  void insert_isolated_vertex_ex (Face_handle f, Vertex_handle v)
  {
    p_arr->_insert_isolated_vertex (p_arr->_face (f), p_arr->_vertex(v));
  }
  
  /*!
   * Relocate all holes and isolated vertices to their proper position,
   * immediately after a face has split due to the insertion of a new halfedge.
   * In case insert_at_vertices_ex() was invoked and indicated that a new face
   * has been created, this function should be called with the halfedge
   * returned by insert_at_vertices_ex().
   * \param new_he The new halfedge that caused the split, such that the new
   *               face lies to its left and the old face to its right.
   */
  void relocate_in_new_face (Halfedge_handle new_he)
  {
    p_arr->_relocate_in_new_face (p_arr->_halfedge (new_he));
    return;
  }

  /*!
   * Move a hole from one face to another.
   * \param from_face The source face.
   * \param to_face The destination face.
   * \param hole A CCB circulator that corresponds to the outer boundary
   *             of the hole to move.
   * \return Whether the hole was successfully moved.
   */
  bool move_hole (Face_handle from_face, Face_handle to_face,
                  Ccb_halfedge_circulator hole)
  {
    typename Arrangement_2::DHoles_iter   it;
    DFace            *from_f =  p_arr->_face (from_face);
    DHalfedge        *he = p_arr->_halfedge (hole->handle());
    DHalfedge        *he_first;
    DHalfedge        *he_curr;
    
    for (it = from_f->holes_begin(); it != from_f->holes_end(); ++it)
    {
      // Go around the boundary of the current hole and check whether he
      // is on this boundary.
      he_first = he_curr = *it;
      do
      {
        if (he_curr == he)
        {
          // Move the current holes which contains he.
          p_arr->_move_hole (from_f, p_arr->_face (to_face), it);
          return (true);
        }
        
        he_curr = he_curr->next();
      } while (he_curr != he_first);
    }

    // If we reached here, the vertex is not contained in from_face.
    return (false);
  }
  
  /*!
   * Move an isolated vertex from one face to another.
   * \param from_face The source face.
   * \param to_face The destination face.
   * \param v The isolated vertex to move.
   * \return Whether the vertex was successfully moved.
   */
  bool move_isolated_vertex (Face_handle from_face, Face_handle to_face,
                              Vertex_handle v)
  {
    typename Arrangement_2::DIsolated_vertices_iter   it;
    DFace            *from_f =  p_arr->_face (from_face);
    DVertex          *iso_v = p_arr->_vertex (v);
     
    for (it = from_f->isolated_vertices_begin();
         it != from_f->isolated_vertices_end(); ++it)
    {
      if (&(*it) == iso_v)
      {
        p_arr->_move_isolated_vertex (from_f, p_arr->_face (to_face), it);
        return (true);
      }
    }

    // If we reached here, the vertex is not contained in from_face.
    return (false);
  }

  /*!
   * Find the vertex in the isolated vertices container of a given face and
   * earse this vertex once it is found.
   * \param f The given face.
   * \param v The isolated vertex.
   * \return Whether the vertex was found and erased or not.
   */
  bool find_and_erase_isolated_vertex (Face_handle f, Vertex_handle v)
  {
    return p_arr->_find_and_erase_isolated_vertex (p_arr->_face (f),
                                                   p_arr->_vertex (v));                                            

  }
 
  /*!
   * Modify the point associated with a given vertex. The point may be
   * geometrically different than the one currently associated with the vertex.
   * \param v The vertex to modify.
   * \param p The new point to associate with v.
   * \return A handle for the modified vertex (same as v).
   */
  Vertex_handle modify_vertex_ex (Vertex_handle v,
                                  const Point_2& p)
  {
    p_arr->_modify_vertex (p_arr->_vertex (v),
                           p);


    return (v);
  }
        
  /*!
   * Modify the x-monotone curve associated with a given edge. The curve may be
   * geometrically different than the one currently associated with the edge.
   * \param e The edge to modify.
   * \param cv The new x-monotone curve to associate with e.
   * \return A handle for the modified edge (same as e).
   */
  Halfedge_handle modify_edge_ex (Halfedge_handle e,
                                  const X_monotone_curve_2& cv)
  {
    p_arr->_modify_edge (p_arr->_halfedge (e),
                         cv);

    return (e);
  }
          
  /*!
   * Split a given edge into two at a given point, and associate the given
   * x-monotone curves with the split edges.
   * \param e The edge to split (one of the pair of twin halfegdes).
   * \param p The split point.
   * \param cv1 The curve that should be associated with the first split edge,
   *            whose source equals e's source and its target is p.
   * \param cv2 The curve that should be associated with the second split edge,
   *            whose source is p and its target equals e's target.
   * \return A handle for the first split halfedge, whose source equals the
   *         source of e, and whose target is the split point.
   */
  Halfedge_handle split_edge_ex (Halfedge_handle e,
                                 const Point_2& p,
                                 const X_monotone_curve_2& cv1, 
                                 const X_monotone_curve_2& cv2)
  {
    DHalfedge*  he = p_arr->_split_edge (p_arr->_halfedge (e),
                                         p,
                                         cv1, cv2);

    CGAL_assertion (he != NULL);
    return (p_arr->_handle_for (he));
  }

  /*!
   * Split a given edge into two at the given vertex, and associate the given
   * x-monotone curves with the split edges.
   * \param e The edge to split (one of the pair of twin halfegdes).
   * \param v The split vertex.
   * \param cv1 The curve that should be associated with the first split edge,

   *            whose source equals e's source and its target is v's point.
   * \param cv2 The curve that should be associated with the second split edge,
   *            whose source is v's point and its target equals e's target.
   * \return A handle for the first split halfedge, whose source equals the
   *         source of e, and whose target is the split vertex v.
   */
  Halfedge_handle split_edge_ex (Halfedge_handle e,
                                 Vertex_handle v,
                                 const X_monotone_curve_2& cv1,
                                 const X_monotone_curve_2& cv2)
  {
    DHalfedge*  he = p_arr->_split_edge (p_arr->_halfedge (e),
                                         p_arr->_vertex (v),
                                         cv1, cv2);

    CGAL_assertion (he != NULL);
    return (p_arr->_handle_for (he));
  }

  /*!
   * Remove a pair of twin halfedges from the arrangement.
   * \param e A handle for one of the halfedges to be removed.
   * \param remove_source Should the source vertex of e be removed if it
   *                      becomes isolated (true by default).
   * \param remove_target Should the target vertex of e be removed if it
   *                      becomes isolated (true by default).
   * \pre In case the removal causes the creation of a new hole, e should 
   *      point at this hole.
   * \return A handle for the remaining face.
   */
  Face_handle remove_edge_ex (Halfedge_handle e,

			      bool remove_source = true,
			      bool remove_target = true)
  {
    DFace*      f = p_arr->_remove_edge (p_arr->_halfedge (e),
					 remove_source, remove_target);
    
    CGAL_assertion (f != NULL);
    return (p_arr->_handle_for (f));
  }
  //@}
};

CGAL_END_NAMESPACE

#endif
