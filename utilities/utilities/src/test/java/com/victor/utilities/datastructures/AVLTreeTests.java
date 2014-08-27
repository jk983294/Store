package com.victor.utilities.datastructures;

import static org.junit.Assert.assertTrue;

import java.util.Collection;

import org.junit.Test;

import com.victor.utilities.datastructures.Utils.*;
import com.victor.utilities.datastructures.tree.AVLTree;
import com.victor.utilities.datastructures.tree.BinarySearchTree;

public class AVLTreeTests {

    @Test
    public void testAVLTree() {
        TestData data = Utils.generateTestData(1000);

        String bstName = "AVL Tree";
        BinarySearchTree<Integer> bst = new AVLTree<Integer>();
        Collection<Integer> bstCollection = bst.toCollection();

        assertTrue(TreeTest.testTree(bst, Type.Integer, bstName, 
                                     data.unsorted, data.invalid));
        assertTrue(JavaCollectionTest.testCollection(bstCollection, Type.Integer, bstName, 
                                                 data.unsorted, data.sorted, data.invalid));
    }
}
