#include "Graph.h"

void Graph::AddNode(shared_ptr<MapGate> node)
{
    if (node == nullptr) return;
    this->nodes[node->GetNodeId()] = node;
}

shared_ptr<MapGate> Graph::GetNode(int id)
{
    if (this->nodes.find(id) == this->nodes.end()) return nullptr;
    return this->nodes.at(id);
}
