package com.victor.utilities.datastructures;

import static org.junit.Assert.assertTrue;

import org.junit.Test;

import com.victor.utilities.datastructures.tree.SuffixTree;

public class SuffixTreeTests {

    @Test
    public void testSuffixTree() {
        String bookkeeper = "bookkeeper";
        SuffixTree<String> tree = new SuffixTree<String>(bookkeeper);
        assertTrue(SuffixTreeTest.suffixTreeTest(tree, bookkeeper));
    }
}
