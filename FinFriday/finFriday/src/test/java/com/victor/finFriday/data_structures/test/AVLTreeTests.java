package com.victor.finFriday.data_structures.test;

import static org.junit.Assert.assertTrue;

import java.util.Collection;

import org.junit.Test;

import com.victor.finFriday.data_structures.AVLTree;
import com.victor.finFriday.data_structures.BinarySearchTree;
import com.victor.finFriday.data_structures.test.common.JavaCollectionTest;
import com.victor.finFriday.data_structures.test.common.TreeTest;
import com.victor.finFriday.data_structures.test.common.Utils;
import com.victor.finFriday.data_structures.test.common.Utils.TestData;
import com.victor.finFriday.data_structures.test.common.Utils.Type;

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
