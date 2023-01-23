package hw4;

import java.util.ArrayList;
import java.util.Iterator;

public class GraphWrapper extends Graph{
	
	public GraphWrapper() {
		super();
	}
	
	public void addNode(String nodeData) {
		super.addNode(nodeData);
	}
	
	public void addEdge(String parentNode, String childNode, String edgeLabel) {
		super.addEdge(parentNode, childNode, edgeLabel);
	}
	
	public Iterator<String> listNodes() {
		ArrayList<String> tmp = super.sortNode();
		Iterator<String> iter = tmp.iterator();
		return iter;
	}
	
	public Iterator<String> listChildren(String parentNode) {
		ArrayList<String> tmp = new ArrayList<String>();
		for (int i = 0; i < super.graph.size(); i++) {
			if (super.graph.get(i).getNodeData() == parentNode) {
				tmp = super.graph.get(i).sortEdge();
			}
		}
		if (tmp == null) {
			return null;
		}
		Iterator<String> iter = tmp.iterator();
		return iter;
	}
}