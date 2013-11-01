#ifndef CGRAPH_H
#define CGRAPH_H

#include <list>
#include <vector>
#include <limits>
#include "ASSERT.h"

using namespace std;

struct Edge
{
    Edge(const unsigned &uToVertex, const double &dValue):
        m_uToVertex(uToVertex), m_dValue(dValue)
    {}
    unsigned m_uToVertex;
    double m_dValue;
};

struct Vertex
{
    Vertex(const double &uValue = numeric_limits<double>::max()):
        m_uValue(uValue)
    {
    }

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
            Edge edgeNew(uToVertex, dValue);
            m_listEdges.push_back(edgeNew);
            bEdgeAdded = true;
        }
        return bEdgeAdded;
    }

    bool DeleteEdgeTo(const unsigned &uToVertex)
    {
        list<Edge>::iterator i_CurrentEdge = m_listEdges.begin();
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
        list<Edge>::const_iterator ci_CurrentEdge = m_listEdges.begin();
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
        ASSERT("Vertex::GetValueOfEdgeTo() there is no such edge", HasEdgeTo(uToVertex) == true);
        list<Edge>::const_iterator ci_CurrentEdge = m_listEdges.begin();
        while(ci_CurrentEdge != m_listEdges.end())
        {
            if(uToVertex == ci_CurrentEdge->m_uToVertex)
            {
                return ci_CurrentEdge->m_dValue;
            }
            ++ci_CurrentEdge;
        }
        ASSERT("Vertex::GetValueOfEdgeTo() reached end of list container (FATAL)", true);
        return 0.0;
    }

    void SetValueOfEdgeTo(const unsigned &uToVertex, const double &dValue)
    {
        ASSERT("Vertex::SetValueOfEdgeTo() there is no such edge", HasEdgeTo(uToVertex) == true);
        list<Edge>::iterator i_CurrentEdge = m_listEdges.begin();
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
        list<Edge>::const_iterator ciEdges = m_listEdges.begin();
        while(ciEdges != m_listEdges.end())
        {
            luListOfNeighbors.push_back(ciEdges->m_uToVertex);
            ++ciEdges;
        }
        return luListOfNeighbors;
    }

private:
    double m_uValue;
    list<Edge> m_listEdges;
};

//typedef pair < unsigned, list<unsigned> > pairVertexWithListOfEdges;

class CGraph
{
public:
    //CGraph(bool bDirected = false);
    CGraph(const unsigned &uNumOfVertices = 0, const bool &bDirected = false):
        m_uNumberOfVertices(uNumOfVertices), m_uNumberOfEdges(0),
        m_bDirected(bDirected)
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
    void AddVertex(const double &uValue = 0)
    {
        m_vectorOfVertices.push_back(uValue);
        ++m_uNumberOfVertices;
    }
    double GetVertexValue(const unsigned &uVertex) const
    {
        ASSERT("CGraph::GetVertexValue() parameter \"uVertex\" out of range", uVertex<m_uNumberOfVertices);
        return m_vectorOfVertices[uVertex].GetValue();
    }
    void SetVertexValue(const unsigned &uVertex, const double &dValue)
    {
        ASSERT("CGraph::SetVertexValue() parameter \"uVertex\" out of range", uVertex<m_uNumberOfVertices);
        m_vectorOfVertices[uVertex].SetValue(dValue);
    }
    void AddEdge(const unsigned &uFromVertex, const unsigned &uToVertex, const double &dValue)
    {
        ASSERT("CGraph::AddEdge() parameter \"uFromVertex\" out of range", uFromVertex<m_uNumberOfVertices);
        ASSERT("CGraph::AddEdge() parameter \"uToVertex\" out of range", uToVertex<m_uNumberOfVertices);
        if(uFromVertex == uToVertex) return;
        if(m_bDirected)
        {
            if(m_vectorOfVertices[uFromVertex].AddEdgeTo(uToVertex, dValue))
            {
                ++m_uNumberOfEdges;
            }
        }
        else
        {
            if( (m_vectorOfVertices[uFromVertex].AddEdgeTo(uToVertex, dValue)) &&
                (m_vectorOfVertices[uToVertex].AddEdgeTo(uFromVertex, dValue)) )
            {
                ++m_uNumberOfEdges;
            }
        }
    }
    bool HasEdge(const unsigned &uFromVertex, const unsigned &uToVertex) const
    {
        ASSERT("CGraph::HasEdge() parameter \"uFromVertex\" out of range", uFromVertex<m_uNumberOfVertices);
        ASSERT("CGraph::HasEdge() parameter \"uToVertex\" out of range", uToVertex<m_uNumberOfVertices);
        return m_vectorOfVertices[uFromVertex].HasEdgeTo(uToVertex);
    }

    void DeleteEdge(const unsigned &uFromVertex, const unsigned &uToVertex)
    {
        ASSERT("CGraph::DeleteEdge() parameter \"uFromVertex\" out of range", uFromVertex<m_uNumberOfVertices);
        ASSERT("CGraph::DeleteEdge() parameter \"uToVertex\" out of range", uToVertex<m_uNumberOfVertices);
        if(m_bDirected)
        {
            if(m_vectorOfVertices[uFromVertex].DeleteEdgeTo(uToVertex))
            {
                --m_uNumberOfEdges;
            }
        }
        else
        {
            if( (m_vectorOfVertices[uFromVertex].DeleteEdgeTo(uToVertex)) &&
                (m_vectorOfVertices[uToVertex].DeleteEdgeTo(uFromVertex)) )
            {
                --m_uNumberOfEdges;
            }
        }

    }
    double GetEdgeValue(const unsigned &uFromVertex, const unsigned &uToVertex) const
    {
        ASSERT("CGraph::GetEdgeValue() parameter \"uFromVertex\" out of range", uFromVertex<m_uNumberOfVertices);
        ASSERT("CGraph::GetEdgeValue() parameter \"uToVertex\" out of range", uToVertex<m_uNumberOfVertices);
        ASSERT("CGraph::GetEdgeValue() there is no such edge", HasEdge(uFromVertex, uToVertex) == true);
        return m_vectorOfVertices[uFromVertex].GetValueOfEdgeTo(uToVertex);
    }
    void SetEdgeValue(const unsigned &uFromVertex, const unsigned &uToVertex, const double &dValue)
    {
        ASSERT("CGraph::SetEdgeValue() parameter \"uFromVertex\" out of range", uFromVertex<m_uNumberOfVertices);
        ASSERT("CGraph::SetEdgeValue() parameter \"uToVertex\" out of range", uToVertex<m_uNumberOfVertices);
        ASSERT("CGraph::SetEdgeValue() there is no such edge", HasEdge(uFromVertex, uToVertex) == true);
        m_vectorOfVertices[uFromVertex].SetValueOfEdgeTo(uToVertex, dValue);
    }
    list<unsigned> GetListOfNeighborVertices(const unsigned &uFromVertex)
    {
        ASSERT("CGraph::GetListOfNeighborVertices() parameter \"uFromVertex\" out of range", uFromVertex<m_uNumberOfVertices);
        return m_vectorOfVertices[uFromVertex].GetListOfNeighbors();
    }
private:
    unsigned m_uNumberOfVertices;
    unsigned m_uNumberOfEdges;
    bool m_bDirected;
    vector< Vertex > m_vectorOfVertices;
};

#endif // CGRAPH_H

