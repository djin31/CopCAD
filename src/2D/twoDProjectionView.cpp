/**Defines interface for Orthographic Projections.
Uses three projection vies as data members */

#include "include/2D/twoDPoint.h"
#include "include/2D/Edge.h"
#include "include/2D/twoDProjection.h"
#include "include/2D/twoDProjectionView.h"
#include "include/3D/threeDPoint.h"
#include "include/3D/threeDObject.h"
#include <iostream>
#include <fstream>
using namespace std;

twoDProjectionView::twoDProjectionView(twoDProjection *f, twoDProjection *t, twoDProjection *s)
{
	frontview = f;
	topview = t;
	sideview = s;
}

twoDProjectionView::twoDProjectionView(twoDProjection *f, twoDProjection *t)
{
	frontview = f;
	topview = t;
	sideview = NULL;
}

PointVector3D twoDProjectionView::pointReconstruction()
{
	twoDPoint frontPointTemp = twoDPoint(0, 0);
	twoDPoint topPointTemp = twoDPoint(0, 0);
	twoDPoint sidePointTemp = twoDPoint(0, 0);
	threeDPoint *retPoint;
	PointVector3D *retVal = new PointVector3D();
	if (sideview == NULL)
	{
		for (int i = 0; i < frontview->PointSet.size(); i++)
		{
			frontPointTemp = *(frontview->PointSet.at(i));
			topPointTemp = *(topview->PointSet.at(i));
			if (abs(frontPointTemp.a - topPointTemp.a) < 0.01)
			{
				retPoint = new threeDPoint(frontPointTemp.a, frontPointTemp.b, topPointTemp.b);
				retVal->push_back(retPoint);
			}
			else
			{
				IncompatibleViews e;
				throw e;
			}
		}
		return *retVal;
	}
	for (int i = 0; i < frontview->PointSet.size(); i++)
	{
		frontPointTemp = *(frontview->PointSet.at(i));
		topPointTemp = *(topview->PointSet.at(i));
		sidePointTemp = *(sideview->PointSet.at(i));
		if ((abs(frontPointTemp.a - topPointTemp.a) < 0.001) && (abs(frontPointTemp.b - sidePointTemp.a) < 0.001) && (abs(sidePointTemp.b - topPointTemp.b) < 0.001))
		{
			retPoint = new threeDPoint(frontPointTemp.a, frontPointTemp.b, topPointTemp.b);
			retVal->push_back(retPoint);
		}
	}
	return *retVal;
}

bool twoDProjectionView::checkEdgeCompatibility_2views(Edge &e)
{
	if (*(topview->PointSet.at(e.start_index)) == *(topview->PointSet.at(e.end_index)))
	{
		return true;
	}
	for (int i = 0; i < topview->EdgeSet.size(); i++)
	{
		if (*(topview->EdgeSet.at(i)) == e)
		{
			return true;
		}
	}
	return false;
}

bool twoDProjectionView::checkEdgeCompatibility_3views(Edge &e)
{
	if (checkEdgeCompatibility_2views(e))
	{
		if (*(sideview->PointSet.at(e.start_index)) == *(sideview->PointSet.at(e.end_index)))
		{
			return true;
		}
		for (int i = 0; i < sideview->EdgeSet.size(); i++)
		{
			if (*(sideview->EdgeSet.at(i)) == e)
			{
				return true;
			}
		}
		return false;
	}
	return false;
}

EdgeVector3D twoDProjectionView::edgeReconstruction()
{
	Edge edgeTemp = Edge(0, 0);
	EdgeVector3D retVal;
	if (sideview == NULL)
	{
		// adds edge visible in front view
		for (int i = 0; i < frontview->EdgeSet.size(); i++)
		{
			if (checkEdgeCompatibility_2views(*(frontview->EdgeSet.at(i))))
				retVal.push_back(frontview->EdgeSet.at(i));
		}

		// adds edge which are not visible in front view
		for (int i = 0; i < topview->EdgeSet.size(); i++)
		{
			edgeTemp = *(topview->EdgeSet.at(i));
			if (*(frontview->PointSet.at(edgeTemp.start_index)) == *(frontview->PointSet.at(edgeTemp.end_index)))
			{
				retVal.push_back(topview->EdgeSet.at(i));
			}
		}

		return retVal;
	}

	// adds edge visible in front view
	for (int i = 0; i < frontview->EdgeSet.size(); i++)
	{
		if (checkEdgeCompatibility_3views(*(frontview->EdgeSet.at(i))))
		{
			retVal.push_back(frontview->EdgeSet.at(i));
		}
	}

	// adds edge which are not visible in front view
	for (int i = 0; i < topview->EdgeSet.size(); i++)
	{
		edgeTemp = *(topview->EdgeSet.at(i));
		if (*(frontview->PointSet.at(edgeTemp.start_index)) == *(frontview->PointSet.at(edgeTemp.end_index)))
		{
			for (int j = 0; j < sideview->EdgeSet.size(); j++)
			{
				if (*(sideview->EdgeSet.at(j)) == edgeTemp)
				{
					retVal.push_back(topview->EdgeSet.at(i));
					break;
				}
			}
		}
	}
	return retVal;
}

threeDObject *twoDProjectionView::create3Dmodel()
{
	threeDObject *retVal = new threeDObject();
	retVal->addPointSet(pointReconstruction());
	retVal->addEdgeSet(edgeReconstruction());
	return retVal;
}

void twoDProjectionView::filewriter(const char *path)
{
	ofstream file;
	file.open(path, ios::out);
	
	if (sideview!=NULL)
		file<<"3\n";
	else
		file<<"2\n";
	
	//printing frontview

	twoDProjection temp = *frontview;
	file << temp.PointSet.size();
	for (int i = 0; i < temp.PointSet.size(); i++)
	{
		file << "\n"
			 << temp.PointSet.at(i)->a << " " << temp.PointSet.at(i)->b;
	}
	file << endl;
	file << temp.EdgeSet.size();
	for (int i = 0; i < temp.EdgeSet.size(); i++)
	{
		file << "\n"
			 << temp.EdgeSet.at(i)->start_index << " " << temp.EdgeSet.at(i)->end_index;
	}
	file << endl;

	//printinf topview
	temp = *topview;
	file << temp.PointSet.size();
	for (int i = 0; i < temp.PointSet.size(); i++)
	{
		file << "\n"
			 << temp.PointSet.at(i)->a << " " << temp.PointSet.at(i)->b;
	}
	file << endl;
	file << temp.EdgeSet.size();
	for (int i = 0; i < temp.EdgeSet.size(); i++)
	{
		file << "\n"
			 << temp.EdgeSet.at(i)->start_index << " " << temp.EdgeSet.at(i)->end_index;
	}

	//printing sideview
	if (sideview != NULL)
	{
		file << endl;
		temp = *sideview;
		file << temp.PointSet.size();
		for (int i = 0; i < temp.PointSet.size(); i++)
		{
			file << "\n"
				 << temp.PointSet.at(i)->a << " " << temp.PointSet.at(i)->b;
		}
		file << endl;
		file << temp.EdgeSet.size();
		for (int i = 0; i < temp.EdgeSet.size(); i++)
		{
			file << "\n"
				 << temp.EdgeSet.at(i)->start_index << " " << temp.EdgeSet.at(i)->end_index;
		}
	}
	file.close();
}