#ifndef _POINTSET2D_HPP_
#define _POINTSET2D_HPP_

#include <fstream>
#include <sstream>
#include <iostream>
#include <ostream>
#include <string>

#include "Abstract/AbstractDataSet.hpp"

class Point2D : public Kaadugal::AbstractDataPoint
{
public:
    Point2D(void) {};
    Point2D(Kaadugal::VPFloat x, Kaadugal::VPFloat y, Kaadugal::VPFloat ClassLabel = -1)
    {
	m_x = x;
	m_y = y;

	m_ClassLabel = ClassLabel;
    };

    Kaadugal::VPFloat m_x;
    Kaadugal::VPFloat m_y;

    int m_ClassLabel; // Always integer starting with 0. -1 means no class label

    friend std::ostream& operator<<(std::ostream &s, const Point2D &p)
    {
	s << "[ " << p.m_x << ", " << p.m_y << "]: " << p.m_ClassLabel << "\n";
	return s;
    };

};

class PointSet2D : public Kaadugal::AbstractDataSet
{
    int m_NumClassLabels;

public:
    PointSet2D(void) {};
    PointSet2D(const std::string& DataFileName)
    {
	Deserialize(DataFileName);
    };

    void Deserialize(const std::string& DataFileName)
    {
	std::fstream DataFile(DataFileName, std::ios::in);
	if(DataFile.is_open())
	{
	    // PointSet2D data files have either 2 or 3 columns per element in the set
	    // First Col: x; Second Col: y[, Third Col: Class Label]
	    std::string Line;
	    while(std::getline(DataFile, Line))
	    {
		// Skip empty lines or lines beginning with #
		if(Line.size() < 1)
		    continue;
		if(Line.data()[0] == '#') // Comment lines
		    continue;

		std::stringstream ss(Line);
		std::vector<std::string> Cols;
		std::string Tmp;
		while(ss >> Tmp)
		    Cols.push_back(Tmp);

		Point2D Row;
		if(Cols.size() == 2)
		{
		    Row = Point2D(std::atof(Cols[0].c_str())
					  , std::atof(Cols[1].c_str())
			);
		}
		else if(Cols.size() == 3)
		{	
		    Row = Point2D(std::atof(Cols[0].c_str())
					  , std::atof(Cols[1].c_str())
					  , std::atof(Cols[2].c_str())
			);
		}
		else
		{
		    std::cout << "[ ERROR ]: Invalid data file. Aborting." << std::endl;
		    m_DataPoints.clear();
		    break;
		}
		std::cout << Row;
		m_DataPoints.push_back(std::make_shared<Point2D>(Row));
	    }
	}
	else
	    std::cout << "[ WARN ]: Unable to open file: " << DataFileName << std::endl;

    };
};

#endif // _POINTSET2D_HPP_