/*
 * Monte Carlo simulation
 *
 *  Created on: 04.11.2013
 *      Author: Iurii Nosov
 *       email: arcnyf@gmail.com
 *
 * Implementation a Monte Carlo simulation that calculates the average shortest path in a graph.
 * The graph is generated using a pseudo-random number generator to produce edges and their costs.
 * Dijkstra’s algorithm is used to calculate shortest path.
 * Made everything in a single "main.cpp" file to make it easier to compile&check!!!
 *
 * !!!Important!!! Contains only implementation needed to achieve main goal of homework.
 * No implementation of unnecessary abilities.
 * p.s. if there any questions/propositions, feel free to write me email.
 */
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <list>
#include <vector>

using namespace std;


// Utility class. Used to check assertions in order to prevent input of incorrect data.
class CASSERT
{
public:
    static void ASSERT_CONDITION(const char *ccMessage, const bool &bCondition)
    {
        if(!bCondition)
        {
            cerr << ccMessage << endl;
            exit(1);
        }
    }
};


// Vertex class. Contains "std::list" of "Edge"s and methods to work with it.
// Can be used only by CGraph class.

class CVertex
{
public:
    friend class CGraph;
    CVertex(const double &uValue = numeric_limits<double>::max()):
        m_uValue(uValue)
    {
    }

private:
    void SetValue(const double &uValue)
    {
        m_uValue = uValue;
    }

    double GetValue() const
    {
        return m_uValue;
    }

    bool AddEdgeTo(const unsigned &uToVertex, const double &dValue)
    {
        bool bEdgeAdded = false;
        if(!HasEdgeTo(uToVertex))
        {
            SEdge Edge(uToVertex, dValue);
            m_listEdges.push_back(Edge);
            bEdgeAdded = true;
        }
        return bEdgeAdded;
    }

    bool DeleteEdgeTo(const unsigned &uToVertex)
    {
        list<SEdge>::iterator i_CurrentEdge = m_listEdges.begin();
        while(i_CurrentEdge != m_listEdges.end())
        {
            if(uToVertex == i_CurrentEdge->m_uToVertex)
            {
                m_listEdges.erase(i_CurrentEdge);
                return true;
            }
            ++i_CurrentEdge;
        }
        return false;
    }

    bool HasEdgeTo(const unsigned &uToVertex) const
    {
        list<SEdge>::const_iterator ci_CurrentEdge = m_listEdges.begin();
        while(ci_CurrentEdge != m_listEdges.end())
        {
            if(uToVertex == ci_CurrentEdge->m_uToVertex)
            {
                return true;
            }
            ++ci_CurrentEdge;
        }
        return false;
    }

    double GetValueOfEdgeTo(const unsigned &uToVertex) const
    {
        list<SEdge>::const_iterator ci_CurrentEdge = m_listEdges.begin();
        while(ci_CurrentEdge != m_listEdges.end())
        {
            if(uToVertex == ci_CurrentEdge->m_uToVertex)
            {
                return ci_CurrentEdge->m_dValue;
            }
            ++ci_CurrentEdge;
        }
        return 0.0;
    }

    void SetValueOfEdgeTo(const unsigned &uToVertex, const double &dValue)
    {
        list<SEdge>::iterator i_CurrentEdge = m_listEdges.begin();
        while(i_CurrentEdge != m_listEdges.end())
        {
            if(uToVertex == i_CurrentEdge->m_uToVertex)
            {
                i_CurrentEdge->m_dValue = dValue;
            }
            ++i_CurrentEdge;
        }
    }

    list<unsigned> GetListOfNeighbors()
    {
        list<unsigned> luListOfNeighbors;
        list<SEdge>::const_iterator ciEdges = m_listEdges.begin();
        while(ciEdges != m_listEdges.end())
        {
            luListOfNeighbors.push_back(ciEdges->m_uToVertex);
            ++ciEdges;
        }
        return luListOfNeighbors;
    }

    struct SEdge
    {
        SEdge(const unsigned &uToVertex, const double &dValue):
            m_uToVertex(uToVertex), m_dValue(dValue)
        {}
        unsigned m_uToVertex;
        double m_dValue;
    };
    double m_uValue;
    list<SEdge> m_listEdges;
};


// Main graph class. Contains "std::vector" of "CVertex"s and methods to work with it.
// As an opened class, uses "CASSERT" class to prevent input of incorrect data.
class CGraph
{
public:
    CGraph(const unsigned &uNumOfVertices = 0):
        m_uNumberOfVertices(uNumOfVertices), m_uNumberOfEdges(0)
    {
        m_vectorOfVertices.resize(m_uNumberOfVertices);
    }

    unsigned GetNumberOfVertices() const // Returns the number of vertices in the graph.
    {
        return m_uNumberOfVertices;
    }

    unsigned GetNumberOfEdges() const // Returns the number of edges in the graph.
    {
        return m_uNumberOfEdges;
    }

    void AddVertex()
    {
        CVertex Vertex;
        m_vectorOfVertices.push_back(Vertex);
        ++m_uNumberOfVertices;
    }

    double GetVertexValue(const unsigned &uVertex) const
    {
        CASSERT::ASSERT_CONDITION("CGraph::GetVertexValue() parameter \"uVertex\" out of range", uVertex<m_uNumberOfVertices);

        return m_vectorOfVertices[uVertex].GetValue();
    }

    void SetVertexValue(const unsigned &uVertex, const double &dValue)
    {
        CASSERT::ASSERT_CONDITION("CGraph::SetVertexValue() parameter \"uVertex\" out of range", uVertex<m_uNumberOfVertices);

        m_vectorOfVertices[uVertex].SetValue(dValue);
    }

    void AddEdge(const unsigned &uFromVertex, const unsigned &uToVertex, const double &dValue)
    {
        CASSERT::ASSERT_CONDITION("CGraph::AddEdge() parameter \"uFromVertex\" out of range", uFromVertex<m_uNumberOfVertices);
        CASSERT::ASSERT_CONDITION("CGraph::AddEdge() parameter \"uToVertex\" out of range", uToVertex<m_uNumberOfVertices);

        if(uFromVertex == uToVertex) return;
        if( (m_vectorOfVertices[uFromVertex].AddEdgeTo(uToVertex, dValue)) &&
                (m_vectorOfVertices[uToVertex].AddEdgeTo(uFromVertex, dValue)) )
        {
            ++m_uNumberOfEdges;
        }
    }

    bool HasEdge(const unsigned &uFromVertex, const unsigned &uToVertex) const
    {
        CASSERT::ASSERT_CONDITION("CGraph::HasEdge() parameter \"uFromVertex\" out of range", uFromVertex<m_uNumberOfVertices);
        CASSERT::ASSERT_CONDITION("CGraph::HasEdge() parameter \"uToVertex\" out of range", uToVertex<m_uNumberOfVertices);

        return m_vectorOfVertices[uFromVertex].HasEdgeTo(uToVertex);
    }

    void DeleteEdge(const unsigned &uFromVertex, const unsigned &uToVertex)
    {
        CASSERT::ASSERT_CONDITION("CGraph::DeleteEdge() parameter \"uFromVertex\" out of range", uFromVertex<m_uNumberOfVertices);
        CASSERT::ASSERT_CONDITION("CGraph::DeleteEdge() parameter \"uToVertex\" out of range", uToVertex<m_uNumberOfVertices);

        if( (m_vectorOfVertices[uFromVertex].DeleteEdgeTo(uToVertex)) &&
                (m_vectorOfVertices[uToVertex].DeleteEdgeTo(uFromVertex)) )
        {
            --m_uNumberOfEdges;
        }
    }

    double GetEdgeValue(const unsigned &uFromVertex, const unsigned &uToVertex) const
    {
        CASSERT::ASSERT_CONDITION("CGraph::GetEdgeValue() parameter \"uFromVertex\" out of range", uFromVertex<m_uNumberOfVertices);
        CASSERT::ASSERT_CONDITION("CGraph::GetEdgeValue() parameter \"uToVertex\" out of range", uToVertex<m_uNumberOfVertices);
        CASSERT::ASSERT_CONDITION("CGraph::GetEdgeValue() there is no such edge", HasEdge(uFromVertex, uToVertex) == true);

        return m_vectorOfVertices[uFromVertex].GetValueOfEdgeTo(uToVertex);
    }

    void SetEdgeValue(const unsigned &uFromVertex, const unsigned &uToVertex, const double &dValue)
    {
        CASSERT::ASSERT_CONDITION("CGraph::SetEdgeValue() parameter \"uFromVertex\" out of range", uFromVertex<m_uNumberOfVertices);
        CASSERT::ASSERT_CONDITION("CGraph::SetEdgeValue() parameter \"uToVertex\" out of range", uToVertex<m_uNumberOfVertices);
        CASSERT::ASSERT_CONDITION("CGraph::SetEdgeValue() there is no such edge", HasEdge(uFromVertex, uToVertex) == true);

        m_vectorOfVertices[uFromVertex].SetValueOfEdgeTo(uToVertex, dValue);
    }

    list<unsigned> GetListOfNeighborVertices(const unsigned &uFromVertex)
    {
        CASSERT::ASSERT_CONDITION("CGraph::GetListOfNeighborVertices() parameter \"uFromVertex\" out of range", uFromVertex<m_uNumberOfVertices);

        return m_vectorOfVertices[uFromVertex].GetListOfNeighbors();
    }
private:
    unsigned m_uNumberOfVertices;
    unsigned m_uNumberOfEdges;
    vector< CVertex > m_vectorOfVertices;
};


// Utility class. Used as a container of pairs of double and unsigned values. First value (double) is a priority key
// Uses "std::list" as a base container.
class CPriorityQueue
{
public:
    bool IsEmpty()
    {
        return m_lPriorityElements.empty();
    }

    void AddElement(const double &cdPriority, const unsigned &cuElement)
    {
        if(m_lPriorityElements.empty())
        {
            m_lPriorityElements.push_back(make_pair(cdPriority, cuElement));
        }
        else
        {
            list< pair<double, unsigned> >::iterator iterPair = m_lPriorityElements.begin();
            while( (iterPair != m_lPriorityElements.end()) && (iterPair->first <= cdPriority) )
            {
                ++iterPair;
            }
            m_lPriorityElements.insert(iterPair, make_pair(cdPriority, cuElement));
        }
    }

    unsigned GetElementWithHighestPriority()
    {
        CASSERT::ASSERT_CONDITION("CPriorityQueue::GetElementWithHighestPriority() priority queue is empty", !m_lPriorityElements.empty());

        unsigned uResult = m_lPriorityElements.begin()->second;
        m_lPriorityElements.erase(m_lPriorityElements.begin());
        return uResult;
    }

    void ChangePriorityOfElement(const unsigned &cuElement, const double &cdNewPriority)
    {
        CASSERT::ASSERT_CONDITION("CPriorityQueue::GetElementWithHighestPriority() \"cuElement\" no such element", HasElement(cuElement));

        RemoveElement(cuElement);
        AddElement(cdNewPriority, cuElement);
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

private:
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

private:
    list<pair<double, unsigned> > m_lPriorityElements;
};


// Class-generator. Has static method to generate graph according to input parameters.
class CGraphGenerator
{
public:
    static CGraph RandomlyGenerateGraph(const unsigned &cuNumberOfVertices, const double &cdEdgeDensity, const double &cdDistanceRange)
    {
        CASSERT::ASSERT_CONDITION("CGraphGenerator::RandomlyGenerateGraph() parameter \"cuNumberOfVertices\" out of range", ((cuNumberOfVertices>1) && (cuNumberOfVertices<=1000)) );
        CASSERT::ASSERT_CONDITION("CGraphGenerator::RandomlyGenerateGraph() parameter \"cdEdgeDensity\" out of range", ((cdEdgeDensity>0.0) && (cdEdgeDensity<=1.0)) );
        CASSERT::ASSERT_CONDITION("CGraphGenerator::RandomlyGenerateGraph() parameter \"cdDistanceRange\" out of range", ((cdDistanceRange>=m_cdMinimumDistance) && (cdDistanceRange<=m_cdMaximumDistance)) );

        static bool m_bRandomizerInitialized = false;
        if(!m_bRandomizerInitialized) // Randomizer initialized only ones
        {
            srand(static_cast<unsigned>(time(NULL)));
            m_bRandomizerInitialized = true;
        }

        CGraph ResultGraph(cuNumberOfVertices);
        for(unsigned uVertexFrom = 0; uVertexFrom<(cuNumberOfVertices - 1); ++uVertexFrom)
        {
            for(unsigned uVertexTo = uVertexFrom + 1; uVertexTo<cuNumberOfVertices; ++uVertexTo)
            {
                if( (rand() % 100) < (cdEdgeDensity * 100) )
                {
                    double dGeneratedDistance = rand()/static_cast<double>(RAND_MAX)*(cdDistanceRange-m_cdMinimumDistance) + m_cdMinimumDistance;
                    ResultGraph.AddEdge(uVertexFrom, uVertexTo, dGeneratedDistance);
                }
            }
        }
        return ResultGraph;
    }
private:
    static const double m_cdMinimumDistance;
    static const double m_cdMaximumDistance;
    CGraphGenerator();

};
const double CGraphGenerator::m_cdMinimumDistance = 1.0;
const double CGraphGenerator::m_cdMaximumDistance = 1000.0;

// Main simulation class. Has static interface method "SimulateOnGraph" to calculate the average shortest path in a graph.
// Uses Dijkstra’s algoritm.
class CMonteCarloSimulation
{
public:
    static double SimulateOnGraph(CGraph &Graph)
    {
        CASSERT::ASSERT_CONDITION("CMonteCarloSimulation::SimulateOnGraph() parameter \"Graph\" has no vertices", (1 <= Graph.GetNumberOfVertices()) );

        return CalculateAverageShortestPathLengthInGraph(Graph);
    }
private:
    static double CalculateAverageShortestPathLengthInGraph(CGraph &Graph, const unsigned &uStartingVertex = 0)
    {
        double dResult = 0;
        unsigned uNumberOfReachableElements = 0; // Needed to get average distance
        unsigned uVertexNumber = Graph.GetNumberOfVertices();
        vector<bool> vecVertexVisited(uVertexNumber, false); // Vector to store "visited" status
        CPriorityQueue PQ; // Container to store reachable pairs of distance/vertex (distance is a "priority"-key)

        Graph.SetVertexValue(uStartingVertex, 0); // Starting vertex has value of 0
        PQ.AddElement(0, uStartingVertex);

        while(!PQ.IsEmpty())
        {
            unsigned uCurrentVertex = PQ.GetElementWithHighestPriority();
            list<unsigned> luNeighborsOfCurrentVertex = Graph.GetListOfNeighborVertices(uCurrentVertex);
            list<unsigned>::const_iterator ciNeighbor = luNeighborsOfCurrentVertex.begin();
            while(ciNeighbor != luNeighborsOfCurrentVertex.end()) // checking all neighbors
            {
                if(!vecVertexVisited[*ciNeighbor])
                {
                    double dVertexValue = Graph.GetVertexValue(*ciNeighbor);
                    double dCurrentVertexValue = Graph.GetVertexValue(uCurrentVertex);
                    double dNewPossibleValue = dCurrentVertexValue + Graph.GetEdgeValue(uCurrentVertex, *ciNeighbor);
                    if( dVertexValue > dNewPossibleValue )
                    {
                        if(PQ.HasElement(*ciNeighbor))
                        {
                            PQ.ChangePriorityOfElement(*ciNeighbor, dNewPossibleValue); // shorter path to an existing vertex found
                        }
                        else
                        {
                            PQ.AddElement(dNewPossibleValue, *ciNeighbor); // path to a new vertex found
                        }
                        Graph.SetVertexValue(*ciNeighbor, dNewPossibleValue);
                    }
                }
                ++ciNeighbor;
            }
            vecVertexVisited[uCurrentVertex] = true;
            dResult += Graph.GetVertexValue(uCurrentVertex);
            ++uNumberOfReachableElements;
        }

        if(1 == uNumberOfReachableElements) // excluding first element as it was our start point
        {
            dResult = 0; // No path from first vertex
        }
        else
        {
            dResult = dResult/(uNumberOfReachableElements-1); // Average distance
        }
        return dResult;
    }
    CMonteCarloSimulation();
};


int main(void)
{
    const unsigned cuNumOfSimulations = 10;
    const unsigned cuNumOfVerticesInGraph = 50;
    const double cdEdgesDensityInGraph = 0.2;
    const double cdRangeDistanceInGraph = 10.0;

    cout << "Monte Carlo simulation." << endl
         << "Calculation of an average shortest path in randomly generated graph." << endl
         << "Number of vertices in graph: " << cuNumOfVerticesInGraph << endl
         << "Distance range in graph: 1.0 to " << cdRangeDistanceInGraph << endl
         << "Edge density in graph: " << cdEdgesDensityInGraph << endl;
    for(unsigned uSimulation = 1; uSimulation <= cuNumOfSimulations; ++uSimulation)
    {
        CGraph Graph = CGraphGenerator::RandomlyGenerateGraph(cuNumOfVerticesInGraph, cdEdgesDensityInGraph, cdRangeDistanceInGraph);
        cout << "Simulation result #" << uSimulation << ": " << CMonteCarloSimulation::SimulateOnGraph(Graph) << endl;
    }

    return 0;
}


