package hw4;

import java.util.ArrayList;
import java.util.Comparator;

public class Node{
	
	// Abstraction Function: A Node object node, has a list with
	// every edges from it and it's value such as "b(4)". The first 
	//time we create a node, the nodeEdge is null
	//
	// Rep Invariant: nodeEdge contains unique terms
	private String nodeData;
	public ArrayList<String> nodeEdge;
	
	/** This is the constructor for Node object
    @param The value of the node
    @requires nodeData != null
	*/
	public Node(String nodeData) {
		this.nodeData = nodeData;
		this.nodeEdge = new ArrayList<String>();
	}
	
	/** This is the function to get the node value
    @return Value of the node
	*/
	public String getNodeData() {
		return this.nodeData;
	}
	
	/** This is the function to check if the node has a edge with given value
    @param An edge's value
    @requires nodeEdge != null
    @return Whether there exists a node with the given value or not
	*/
	public Boolean findEdge(String childNode, String edgeLabel) {
		String tmp = "";
		if (edgeLabel == null) {
			tmp = tmp + childNode + "(" + ")";
		}
		else {
			tmp = tmp + childNode + "(" + edgeLabel + ")";
		}
		return nodeEdge.contains(tmp);
	}
	
	/** This is the function to return a sorted node by edges
    @requires Node has edges
    @return A new node with edges sorted in lexicographical order
	*/	
	public ArrayList<String> sortEdge() {
		ArrayList<String> tmp = this.nodeEdge;
		tmp.sort(Comparator.naturalOrder());
		return tmp;
	}
}
