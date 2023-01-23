package hw4;

import static org.junit.Assert.*;
import java.util.ArrayList;

import org.junit.Test;

public class GraphTest {

	@Test
	public void testConstructor() {
		new Graph();
	}
	
	@Test
	public void testNode() {
		Graph tmp = new Graph();
		tmp.addNode(null);
		tmp.addNode("c");
		tmp.addNode("a");
		tmp.addNode("a");
		tmp.addNode("b");
		int size = tmp.nodeNum();
	    assertEquals("length should be 3", size, 3);
	    ArrayList<String> sorted_Graph = tmp.sortNode();	    
	    assertEquals("after sort check", sorted_Graph.get(0), "a");
	    assertEquals("after sort check", sorted_Graph.get(1), "b");
	    assertEquals("after sort check", sorted_Graph.get(2), "c"); 
	}
	
	@Test
	public void testEdge() {
		Graph tmp = new Graph();
		tmp.addNode(null);
		tmp.addNode("a");
		tmp.addNode("b");
		tmp.addEdge("a", "b", null);
		int num = tmp.edgeNum();

	    assertEquals("num should be 1", num, 1);
	    
		tmp.addEdge("a", "b", "1");
		tmp.addEdge("a", "b", "1");
		num = tmp.edgeNum();
	    assertEquals("num should be 2", num, 2);
	    
	    tmp.addEdge("b", "a", "1");
	    tmp.addEdge("b", "a", "1");
		num = tmp.edgeNum();
	    assertEquals("num should be 3", num, 3);
	    
	    tmp.addEdge("a", "a", "1");
	    tmp.addEdge("b", "b", "1");
		num = tmp.edgeNum();
	    assertEquals("num should be 5", num, 5);
	    
	    ArrayList<String> test_list = tmp.graph.get(0).sortEdge();
	    assertEquals("after sort check", test_list.get(0), "a(1)");
	    assertEquals("after sort check", test_list.get(1), "b()");
	    assertEquals("after sort check", test_list.get(2), "b(1)"); 
	}
}
