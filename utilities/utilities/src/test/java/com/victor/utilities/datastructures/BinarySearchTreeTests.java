package com.victor.utilities.datastructures;

import static org.junit.Assert.assertTrue;

import java.util.Collection;

import org.junit.Test;

import com.victor.utilities.datastructures.Utils.*;
import com.victor.utilities.datastructures.tree.BinarySearchTree;

public class BinarySearchTreeTests {

    @Test
    public void testBST() {
        TestData data = Utils.generateTestData(1000);

        String bstName = "BST";
        BinarySearchTree<Integer> bst = new BinarySearchTree<Integer>();
        Collection<Integer> bstCollection = bst.toCollection();

        assertTrue(TreeTest.testTree(bst, Type.Integer, bstName,
                                     data.unsorted, data.invalid));
        assertTrue(JavaCollectionTest.testCollection(bstCollection, Type.Integer, bstName,
                                                 data.unsorted, data.sorted, data.invalid));
    }
}
