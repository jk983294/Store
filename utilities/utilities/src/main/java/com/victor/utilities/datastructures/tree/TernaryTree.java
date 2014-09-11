package com.victor.utilities.datastructures.tree;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.Set;

public class TernaryTree {
	private final int suggestionCnt = 5;
    private Node root;

    private void add(String s, String orig, int pos, Node node) {
        final char currentChar = s.charAt(pos);
        if (currentChar < node.character) {
            if (node.left == null) {
                node.left = new Node(currentChar, false);
            }
            add(s, orig, pos, node.left);
        } else if (currentChar > node.character) {
            if (node.right == null) {
                node.right = new Node(currentChar, false);
            }
            add(s, orig, pos, node.right);
        } else {
            if (pos + 1 == s.length()) {
                node.wordEnd = true;
                node.words.add(orig);
            } else {
                if (node.middle == null) {
                    node.middle = new Node(s.charAt(pos + 1), false);
                }
                add(s, orig, pos + 1, node.middle);
            }
        }
    }

    public void add(String s) {
        if (s == null || s.length() == 0) {
            throw new IllegalArgumentException("Don't add a null or empty string");
        }
        int pos = 0;
        String su = s.toUpperCase(); 
        if (root == null) {
            root = new Node(su.charAt(0), false);	//first string's 0th char is root
        }
        add(su, s, pos, root);
    }

    public boolean contains(String s) {
        if (s == null || s.length() == 0) {
            throw new IllegalArgumentException();
        }
        int pos = 0;
        Node node = root;
        while (node != null) {
            final char c = Character.toUpperCase(s.charAt(pos));
            if (c < node.character) {
                node = node.left;
            } else if (c > node.character) {
                node = node.right;
            } else {
                if (++pos == s.length()) {
                    return node.wordEnd;
                }
                node = node.middle;
            }
        }
        return false;
    }

    public List<String> getCompletionsFor(String orig) {
        List<String> completions = null;
        String s = orig.toUpperCase();
        if (s == null || s.length() == 0) {
            throw new IllegalArgumentException();
        }
        int pos = 0;
        Node node = root;
        while (node != null) {
            final char c = s.charAt(pos);
            if (c < node.character) {
                node = node.left;
            } else if (c > node.character) {
                node = node.right;
            } else {
                if (++pos == s.length()) {
                    if (!node.wordEnd) {
                        node = node.middle;
                    }
                    completions = new ArrayList<String>();
                    collectCompletions(node, completions);
                    return completions;
                }
                node = node.middle;
            }
        }
        return completions;
    }

    private void collectCompletions(Node node, List<String> completions) {
    	if(completions.size() >= suggestionCnt) return;
        if (node != null) {
            if (node.wordEnd) {     
            	if(completions.size() +  node.words.size() <= suggestionCnt) 
            		completions.addAll(node.words);  
            	else {
            		int need2add = suggestionCnt - completions.size();
            		Iterator<String> it = node.words.iterator();
            		while (need2add>0) {
            		  completions.add(it.next());
            		  --need2add;
            		}				
				}
            }
            collectCompletions(node.middle, completions);
            collectCompletions(node.left, completions);
            collectCompletions(node.right, completions);
        }
    }
    
    public void BFS(){
    	Queue<Node> queue = new LinkedList<Node>();
    	if(root != null) queue.offer(root);
    	int levelcnt = 1, nextlevelcnt = 0;  	
    	while(levelcnt > 0){
    		Node current = queue.poll();
    		System.out.print(current.toString());
    		if(current.left != null){ queue.offer(current.left); nextlevelcnt++; }
    		if(current.middle != null){ queue.offer(current.middle); nextlevelcnt++; }
    		if(current.right != null){ queue.offer(current.right); nextlevelcnt++; }
    		levelcnt--;
    		if(levelcnt == 0){
    			levelcnt = nextlevelcnt;
    			nextlevelcnt = 0;
    			System.out.println();
    		}
    	}
    }

    class Node {
        char character;
        boolean wordEnd;
        Set<String> words = new HashSet<String>();
        public Node left, middle, right;

        Node(char character, boolean wordEnd) {
            this.character = character;
            this.wordEnd = wordEnd;
        }

		@Override
		public String toString() {
			return "Node [character=" + character + ", wordEnd=" + wordEnd
					+ ", words=" + words + ", left=" + (left!=null ?left.character:"null") + ", middle="
					+ (middle!=null ?middle.character:"null") + ", right=" + (right!=null ?right.character:"null") + "]";
		}
        
    }

}
