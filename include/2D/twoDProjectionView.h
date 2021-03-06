/**Defines interface for Orthographic Projections.
Uses three projection vies as data members */

//=================================
// include guard
#ifndef _twoDProjectionView_h
#define _twoDProjectionView_h

//=================================
// included dependencies
#include "include/common.h"

class twoDProjection;
class threeDObject;

class IncompatibleViews : public std::exception
{
  public:
	const char *what() const throw()
	{
		return "Incompatible Views!\n";
	}
};

class twoDProjectionView
{
  private:
	twoDProjection *frontview;
	twoDProjection *topview;
	twoDProjection *sideview;

  public:

	/**Constructor for initiatilising object with three views*/
	twoDProjectionView(twoDProjection *f, twoDProjection *t, twoDProjection *s);

	/**Constructor for intialising object with two view*/
	twoDProjectionView(twoDProjection *f, twoDProjection *t);

	/**Projects a point in 3D space*/
	PointVector3D pointReconstruction();

	/**Checks whether an edge is compatible across projections*/
	bool checkEdgeCompatibility_2views(Edge &e);

	/**Checks whether an edge is compatible across projections*/
	bool checkEdgeCompatibility_3views(Edge &e);

	/**Projects edges in 3D space*/
	EdgeVector3D edgeReconstruction();

	/**Removes edges which are redundant - To be implemented in future versions if any*/
	EdgeVector3D pruneEdges(EdgeVector3D const &e);

	/**Function to convert an Orthographic Porojections Drawing to 3D Model*/
	threeDObject *create3Dmodel();

	/**Writes the object to file*/
	void filewriter(const char * path);

	// Friend Classes
	friend class ProjectionWindow;
};
#endif