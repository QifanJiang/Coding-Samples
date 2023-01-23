package hw4;

import java.util.ArrayList;
import java.util.Comparator;

public class Graph {
	
	// Abstraction Function: A Graph object graph, represents a 
	// list with every Node object node. The first time we create 
	// a graph without nodes, the graph is null
	//
	// Rep Invariant: graph contains no duplicates and no nulls
	public ArrayList<Node> graph;
	
	/** This is the constructor for Graph object
    @effect Construct a new graph
	*/
	public Graph() {
		this.graph = new ArrayList<Node>();
	}
	
	/** This is the function to add a new node to the graph
    @param The value of the node
    @requires nodeData != null
    @effect Adding a new node to the graph
	*/
	public void addNode(String nodeData) {
		if(!findNode(nodeData)) {
			graph.add(new Node(nodeData));
		}
		checkRep();
	}
	
	/** This is the function to add a new edge to the graph
    @param Where the edge start from, where the edge end with, and what's its label
    @requires parentNode and childNode both exists in the graph
    @effect Adding a new edge to the graph
	*/
	public void addEdge(String parentNode, String childNode, String edgeLabel) {
		if(findNode(parentNode) && findNode(childNode)) {
			for (int i = 0; i < graph.size(); i++) {
				if (graph.get(i).getNodeData() == parentNode) {
					if (graph.get(i).findEdge(childNode, edgeLabel)) {
						break;
					}
					String tmp = "";
					if (edgeLabel == null) {
						tmp = tmp + childNode + "(" + ")";
					}
					else {
						tmp = tmp + childNode + "(" + edgeLabel + ")";
					}
					graph.get(i).nodeEdge.add(tmp);
				}
			}
		}
	}
	
	/** This is the function to check if the graph has a node with given value
    @param A node's value
    @requires nodeData != null
    @return Whether there exists a node with the given value or not
	*/
	public boolean findNode(String nodeData) {
		if (nodeData == null) {
			return true;
		}
		boolean existance = false;
		for (int i = 0; i < graph.size(); i++) {
			if (graph.get(i).getNodeData() == nodeData) {
				existance = true;
				break;
			}
		}
		return existance;
	}
	
	/** This is the function to return a sorted graph by nodes
    @requires Graph != null
    @return A list with node values sorted in lexicographical order
	*/	
	public ArrayList<String> sortNode() {
		ArrayList<String> tmp = new ArrayList<String>();
		for (int i = 0; i < graph.size(); i++) {
			tmp.add(graph.get(i).getNodeData());
		}
		tmp.sort(Comparator.naturalOrder());
		return tmp;
	}
	
	/** This is the function to get the total number of nodes in the graph
    @return The total number of nodes in the graph
	*/
	public Integer nodeNum() {
		return graph.size();
	}
	
	/** This is the function to get the total number of edges in the graph
    @return The total number of nodes in the graph
	*/
	public Integer edgeNum() {
		int tmp = 0;
		for (int i = 0; i < graph.size(); i++) {
			if (graph.get(i).nodeEdge != null) {
				tmp = tmp + graph.get(i).nodeEdge.size();
			}
		}
		return tmp;
	}
	/**
     * Checks that the graph doesn't contain null node 
     * (duplicates are checked in findNode)
     */
    // Throws a RuntimeException if the rep invariant is violated.
    private void checkRep() throws RuntimeException {
        for (int i = 0; i < graph.size(); i++) {
        	if (graph.get(i).getNodeData() == null)
        		throw new RuntimeException("coeffs == null");
        }
    }
	
	
}
