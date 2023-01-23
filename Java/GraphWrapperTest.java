package hw4;

import static org.junit.Assert.*;
import java.util.ArrayList;
import java.util.Iterator;

import org.junit.Test;

public class GraphWrapperTest {

	@Test
	public void testConstructor() {
		new GraphWrapper();
	}
	
	@Test
	public void testNode() {
		Graph tmp = new GraphWrapper();
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
		GraphWrapper tmp = new GraphWrapper();
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
	
	@Test
	public void testIter() {
		GraphWrapper tmp = new GraphWrapper();
		tmp.addNode("a");
		tmp.addNode("b");
		tmp.addNode("c");
		tmp.addEdge("a", "b", "1");
		tmp.addEdge("a", "b", "2");
		tmp.addEdge("a", "c", "1");
		
		Iterator<String> iter1 = tmp.listChildren("a");
	    assertEquals("after sort check", iter1.next(), "b(1)");
	    assertEquals("after sort check", iter1.next(), "b(2)");
	    assertEquals("after sort check", iter1.next(), "c(1)");
		
	}
}
