#include "CGraph.h"
#include "ASSERT.h"

//CGraph::CGraph(bool bDirected):
//    m_uNumberOfVertices(0), m_uNumberOfEdges(0),
//    m_bDirected(bDirected)
//{
//}

CGraph::CGraph(const unsigned &uNumOfVertices, const bool &bDirected):
    m_uNumberOfVertices(uNumOfVertices), m_uNumberOfEdges(0),
    m_bDirected(bDirected)
{
    m_vectorOfVertices.resize(m_uNumberOfVertices);
}

unsigned CGraph::GetNumberOfVertices() const
{
    return m_uNumberOfVertices;
}

unsigned CGraph::GetNumberOfEdges() const
{
    return m_uNumberOfEdges;
}

void CGraph::AddVertex(const double &uValue)
{
    m_vectorOfVertices.push_back(uValue);
    ++m_uNumberOfVertices;
}

void CGraph::AddEdge(const unsigned &uFromVertex, const unsigned &uToVertex, const double &dDistance)
{
    ASSERT("CGraph::AddEdge() parameter \"uFromVertex\" out of range", uFromVertex<m_uNumberOfVertices);
    ASSERT("CGraph::AddEdge() parameter \"uToVertex\" out of range", uToVertex<m_uNumberOfVertices);
    if(uFromVertex == uToVertex) return;
    if(m_bDirected)
    {
        if(m_vectorOfVertices[uFromVertex].AddEdgeTo(uToVertex, dDistance))
        {
            ++m_uNumberOfEdges;
        }
    }
    else
    {
        if( (m_vectorOfVertices[uFromVertex].AddEdgeTo(uToVertex, dDistance)) &&
            (m_vectorOfVertices[uToVertex].AddEdgeTo(uFromVertex, dDistance)) )
        {
            ++m_uNumberOfEdges;
        }
    }
}

void CGraph::DeleteEdge(const unsigned &uFromVertex, const unsigned &uToVertex)
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

bool CGraph::HasEdge(const unsigned &uFromVertex, const unsigned &uToVertex) const
{
    ASSERT("CGraph::HasEdge() parameter \"uFromVertex\" out of range", uFromVertex<m_uNumberOfVertices);
    ASSERT("CGraph::HasEdge() parameter \"uToVertex\" out of range", uToVertex<m_uNumberOfVertices);
    //if (uFromVertex == uToVertex) return true;
    return m_vectorOfVertices[uFromVertex].HasEdgeTo(uToVertex);
}

double CGraph::GetEdgeValue(const unsigned &uFromVertex, const unsigned &uToVertex) const
{
    ASSERT("CGraph::GetEdgeValue() parameter \"uFromVertex\" out of range", uFromVertex<m_uNumberOfVertices);
    ASSERT("CGraph::GetEdgeValue() parameter \"uToVertex\" out of range", uToVertex<m_uNumberOfVertices);
    ASSERT("CGraph::GetEdgeValue() there is no such edge", HasEdge(uFromVertex, uToVertex) == true);
    return m_vectorOfVertices[uFromVertex].GetValueOfEdgeTo(uToVertex);
}

void CGraph::SetEdgeValue(const unsigned &uFromVertex, const unsigned &uToVertex, const double &dValue)
{
    ASSERT("CGraph::SetEdgeValue() parameter \"uFromVertex\" out of range", uFromVertex<m_uNumberOfVertices);
    ASSERT("CGraph::SetEdgeValue() parameter \"uToVertex\" out of range", uToVertex<m_uNumberOfVertices);
    ASSERT("CGraph::SetEdgeValue() there is no such edge", HasEdge(uFromVertex, uToVertex) == true);
    m_vectorOfVertices[uFromVertex].SetValueOfEdgeTo(uToVertex, dValue);
}

