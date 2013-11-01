#include <iostream>
#include <iomanip>
#include <ctime>
#include "CGraph.h"
#include "ASSERT.h"

using namespace std;

void PrintGraphAsList(const CGraph &Graph)
{
    unsigned uVertexNumber = Graph.GetNumberOfVertices();
    cout << "Vertices: " << uVertexNumber << endl;
    cout << "Edges: " << Graph.GetNumberOfEdges() << endl;
    for(unsigned uVertexFrom = 0; uVertexFrom<uVertexNumber; ++uVertexFrom)
    {
        cout << uVertexFrom << "(" << Graph.GetVertexValue(uVertexFrom) << ")-> ";
        for(unsigned uVertexTo = 0; uVertexTo<uVertexNumber; ++uVertexTo)
        {
            if(Graph.HasEdge(uVertexFrom, uVertexTo))
            {
                cout<< uVertexTo << "(" << Graph.GetEdgeValue(uVertexFrom, uVertexTo) << "); ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

class CGraphGenerator
{
public:
    static CGraph RandomlyGenerateGraph(const unsigned &cuNumberOfVertices, const double &cdEdgeDensity, const double &cdDistanceRange)
    {
        ASSERT("CGraphGenerator::RandomlyGenerateGraph() parameter \"cuNumberOfVertices\" out of range", ((cuNumberOfVertices>1) && (cuNumberOfVertices<=1000)) );
        ASSERT("CGraphGenerator::RandomlyGenerateGraph() parameter \"cdEdgeDensity\" out of range", ((cdEdgeDensity>0.0) && (cdEdgeDensity<=1.0)) );
        ASSERT("CGraphGenerator::RandomlyGenerateGraph() parameter \"cdDistanceRange\" out of range", ((cdDistanceRange>=m_cdMinimumDistanse) && (cdDistanceRange<=10.0)) );

        CGraph ResultGraph(cuNumberOfVertices);
        for(unsigned uVertexFrom = 0; uVertexFrom<(cuNumberOfVertices - 1); ++uVertexFrom)
        {
            for(unsigned uVertexTo = uVertexFrom + 1; uVertexTo<cuNumberOfVertices; ++uVertexTo)
            {
                if( (rand() % 100) < (cdEdgeDensity * 100) )
                {
                    double dGeneratedDistance = rand()/static_cast<double>(RAND_MAX)*(cdDistanceRange-m_cdMinimumDistanse) + m_cdMinimumDistanse;
                    ResultGraph.AddEdge(uVertexFrom, uVertexTo, dGeneratedDistance);
                }
            }
        }
        return ResultGraph;
    }
private:
    CGraphGenerator();
    static const double m_cdMinimumDistanse = 1.0;
};


class CMonteCarloSimulation
{
public:
    static double CalculateAverageShortestPathLengthInRandomlyGeneratedGraph(const unsigned &cuNumberOfVertices, const double &cdEdgeDensity, const double &cdDistanceRange)
    {
        CGraph GeneratedGraph = CGraphGenerator::RandomlyGenerateGraph(cuNumberOfVertices, cdEdgeDensity, cdDistanceRange);
        PrintGraphAsList(GeneratedGraph);
        //cout << "Generated edges: " << GeneratedGraph.GetNumberOfEdges() << endl;
        return AverageShortestPathInGraphFromVertex(GeneratedGraph, 0);
    }
private:
    static double AverageShortestPathInGraphFromVertex(CGraph &Graph, const unsigned &uStartingVertex = 0)
    {
        double dResult = 0;
        Graph.SetVertexValue(uStartingVertex, 0); // Starting vertex has distanse value of 0
        unsigned uCurrentIndex = uStartingVertex;


        return dResult;
    }
};

int main()
{
    srand(time(NULL));
    const unsigned cuNumberOfSimulations = 1;
    const unsigned cuNumberOfVerticesInGraph = 5;
    const double cdEdgesDensityInGraph = 0.4;
    const double cdDistanceRangeInGraph = 1.0;

    cout << "Monte Carlo simulation." << endl
         << "Calculation of an average shortest path in randomly generated graph." << endl
         << "Number of vertices in graph: " << cuNumberOfVerticesInGraph << endl
         << "Edge density in graph: " << cdEdgesDensityInGraph << endl
         << "Distanse range in graph: 1.0 to " << cdDistanceRangeInGraph << endl;
    for(unsigned uSimulation = 0; uSimulation < cuNumberOfSimulations; ++uSimulation)
    {
        double dSimulationResult = CMonteCarloSimulation::CalculateAverageShortestPathLengthInRandomlyGeneratedGraph(cuNumberOfVerticesInGraph, cdEdgesDensityInGraph, cdDistanceRangeInGraph);
        cout << "Simulation result #" << uSimulation + 1 << ": " << dSimulationResult << endl;
    }

//    CGraph ExperimentalGraph = CGraphGenerator::RandomlyGenerateGraph(cuNumberOfVerticies, 0.4, 50);
//    PrintGraphAsList(ExperimentalGraph);
//    CGraph ExperimentalGraph1 = CGraphGenerator::RandomlyGenerateGraph(15, 0.2, 50);
//    PrintGraphAsList(ExperimentalGraph1);

    return 0;
}


