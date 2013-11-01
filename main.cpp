#include <iostream>
#include <iomanip>
#include <ctime>
#include <map>
#include "CGraph.h"
#include "ASSERT.h"

using namespace std;

class CPriorityQueue
{
public:
    bool IsEmpty()
    {
        return m_lPriorityElements.empty();
    }

    void PushElement(const double &cdPriority, const unsigned &cuElement)
    {
        if(m_lPriorityElements.empty())
        {
            m_lPriorityElements.push_back(make_pair(cdPriority, cuElement));
        }
        else
        {
            list< pair<double, unsigned> >::iterator iterPair = m_lPriorityElements.begin();
            while((iterPair->first <= cdPriority) && (iterPair != m_lPriorityElements.end()))
            {
                ++iterPair;
            }
            m_lPriorityElements.insert(iterPair, make_pair(cdPriority, cuElement));
        }
    }

    unsigned PopElement()
    {
        unsigned uResult = m_lPriorityElements.begin()->second;
        RemoveElement(uResult);
        return uResult;
    }

    void RemoveElement(const unsigned &cuElement)
    {
        list< pair<double, unsigned> >::iterator iterPair = m_lPriorityElements.begin();
        while(iterPair != m_lPriorityElements.end())
        {
            if(iterPair->second == cuElement)
            {
                m_lPriorityElements.erase(iterPair);
                break;
            }
            ++iterPair;
        }
    }

    void ChangePriority(const unsigned &cuElement, const double &cdNewPriority)
    {
        RemoveElement(cuElement);
        PushElement(cdNewPriority, cuElement);
    }

    bool HasElement(const unsigned &cuElement)
    {
        list< pair<double, unsigned> >::const_iterator citerPair = m_lPriorityElements.begin();
        while(citerPair != m_lPriorityElements.end())
        {
            if(citerPair->second == cuElement) return true;
            ++citerPair;
        }
        return false;
    }

    void PrintQueue()
    {
        list< pair<double, unsigned> >::const_iterator citerPair = m_lPriorityElements.begin();
        while (citerPair != m_lPriorityElements.end())
        {
            cout << citerPair->first << "(" << citerPair->second << ") ";
            ++citerPair;
        }
        cout << endl;
    }
private:
    list<pair<double, unsigned> > m_lPriorityElements;
};

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
        ASSERT("CGraphGenerator::RandomlyGenerateGraph() parameter \"cdDistanceRange\" out of range", ((cdDistanceRange>=m_cdMinimumdistance) && (cdDistanceRange<=10.0)) );

        CGraph ResultGraph(cuNumberOfVertices);
        for(unsigned uVertexFrom = 0; uVertexFrom<(cuNumberOfVertices - 1); ++uVertexFrom)
        {
            for(unsigned uVertexTo = uVertexFrom + 1; uVertexTo<cuNumberOfVertices; ++uVertexTo)
            {
                if( (rand() % 100) < (cdEdgeDensity * 100) )
                {
                    double dGeneratedDistance = rand()/static_cast<double>(RAND_MAX)*(cdDistanceRange-m_cdMinimumdistance) + m_cdMinimumdistance;
                    ResultGraph.AddEdge(uVertexFrom, uVertexTo, dGeneratedDistance);
                }
            }
        }
        return ResultGraph;
    }
private:
    CGraphGenerator();
    static const double m_cdMinimumdistance = 1.0;
};

class CMonteCarloSimulation
{
public:
    static double CalculateAverageShortestPathLengthInRandomlyGeneratedGraph(const unsigned &cuNumberOfVertices, const double &cdEdgeDensity, const double &cdDistanceRange)
    {
        CGraph GeneratedGraph = CGraphGenerator::RandomlyGenerateGraph(cuNumberOfVertices, cdEdgeDensity, cdDistanceRange);
        //PrintGraphAsList(GeneratedGraph);
        //cout << "Generated edges: " << GeneratedGraph.GetNumberOfEdges() << endl;
        return AverageShortestPathInGraphFromVertex(GeneratedGraph, 0);
    }
//private:
    static double AverageShortestPathInGraphFromVertex(CGraph &Graph, const unsigned &uStartingVertex = 0)
    {
        double dResult = 0;
        unsigned uNumberOfReachableElements = 0;
        Graph.SetVertexValue(uStartingVertex, 0); // Starting vertex has distance value of 0
        unsigned uVertexNumber = Graph.GetNumberOfVertices();
        bool *pVertexVisited = new bool[uVertexNumber];
        for(unsigned uIndex = 0; uIndex<uVertexNumber; ++uIndex)
        {
            pVertexVisited[uIndex] = false;
        }

        //multimap <double, unsigned> mmValueToIndexMap;
        CPriorityQueue PQ;
//        PQ.PushElement();
        unsigned uCurrentVertex = uStartingVertex;

        while(1)
        {
            double dCurrentVertexValue = Graph.GetVertexValue(uCurrentVertex);
            list<unsigned> luNeighborsOfCurrentVertex = Graph.GetListOfNeighborVertices(uCurrentVertex);
            list<unsigned>::const_iterator ciNeighbor = luNeighborsOfCurrentVertex.begin();
            while(ciNeighbor != luNeighborsOfCurrentVertex.end())
            {
                double dEdgeValue = Graph.GetEdgeValue(uCurrentVertex, *ciNeighbor);
                double dVertexValue = Graph.GetVertexValue(*ciNeighbor);
                double dNewPossibleValue = dCurrentVertexValue + dEdgeValue;
                if( (!pVertexVisited[*ciNeighbor]) && (dVertexValue > dNewPossibleValue) )
                {
                    if(PQ.HasElement(*ciNeighbor))
                    {
                        PQ.ChangePriority(*ciNeighbor, dNewPossibleValue);
                    }
                    else
                    {
                        PQ.PushElement(dNewPossibleValue, *ciNeighbor);
                    }
                    Graph.SetVertexValue(*ciNeighbor, dNewPossibleValue);
                }
                ++ciNeighbor;
            }

            pVertexVisited[uCurrentVertex] = true;
            PQ.RemoveElement(uCurrentVertex);
            dResult += Graph.GetVertexValue(uCurrentVertex);
            ++uNumberOfReachableElements;
            if(PQ.IsEmpty()) break;
            uCurrentVertex = PQ.PopElement();
        }

        delete []pVertexVisited;
        return dResult/(uNumberOfReachableElements-1);
    }
};

int main()
{
    srand(time(NULL));
    const unsigned cuNumberOfSimulations = 10;
    const unsigned cuNumberOfVerticesInGraph = 500;
    const double cdEdgesDensityInGraph = 0.4;
    const double cdDistanceRangeInGraph = 1.0;

//    CGraph ExperimentalGraph(4);
//    ExperimentalGraph.AddEdge(0, 1, 1);
//    ExperimentalGraph.AddEdge(1, 2, 1);
//    ExperimentalGraph.AddEdge(0, 3, 1);
//    cout << CMonteCarloSimulation::AverageShortestPathInGraphFromVertex(ExperimentalGraph) << endl;

    cout << "Monte Carlo simulation." << endl
         << "Calculation of an average shortest path in randomly generated graph." << endl
         << "Number of vertices in graph: " << cuNumberOfVerticesInGraph << endl
         << "Edge density in graph: " << cdEdgesDensityInGraph << endl
         << "distance range in graph: 1.0 to " << cdDistanceRangeInGraph << endl;
    for(unsigned uSimulation = 0; uSimulation < cuNumberOfSimulations; ++uSimulation)
    {
        double dSimulationResult = CMonteCarloSimulation::CalculateAverageShortestPathLengthInRandomlyGeneratedGraph(cuNumberOfVerticesInGraph, cdEdgesDensityInGraph, cdDistanceRangeInGraph);
        cout << "Simulation result #" << uSimulation + 1 << ": " << dSimulationResult << endl;
    }

//    CPriorityQueue PQ;
//    PQ.PushElement(5.5, 0);
//    PQ.PushElement(5.4, 1);
//    PQ.PushElement(0.5, 2);
//    PQ.PushElement(10.4, 3);
//    PQ.PushElement(5.6, 4);
//    PQ.PushElement(1.4, 5);
//    PQ.PushElement(-0.5, 6);
//    PQ.PushElement(101.4, 7);
//    PQ.RemoveElement(4);
//    PQ.ChangePriority(2, -10.0);
//    PQ.PrintQueue();

//    CGraph ExperimentalGraph = CGraphGenerator::RandomlyGenerateGraph(cuNumberOfVerticies, 0.4, 50);
//    PrintGraphAsList(ExperimentalGraph);
//    CGraph ExperimentalGraph1 = CGraphGenerator::RandomlyGenerateGraph(15, 0.2, 50);
//    PrintGraphAsList(ExperimentalGraph1);

    return 0;
}


