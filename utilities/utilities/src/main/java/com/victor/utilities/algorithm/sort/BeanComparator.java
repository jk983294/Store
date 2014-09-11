package com.victor.utilities.algorithm.sort;

import java.util.Comparator;
import java.lang.reflect.Method;
import java.lang.reflect.Type;

public class BeanComparator<T> implements Comparator<T>
{
  public static String DEFAULT_METHOD_NAME = "toString";

  public static final int ASCENDING = 1; 
  public static final int DESCENDING = -1;
  public static int DEFAULT_ORDER = ASCENDING;

  private static final java.lang.Object[] NO_ARGS = null;

  private String methodName;
  private Method getterMethod;
  private int order;

  public BeanComparator(Class classToCompare)
    throws NoSuchMethodException
  {
    this(classToCompare, DEFAULT_METHOD_NAME, DEFAULT_ORDER);
  }

  public BeanComparator(Class classToCompare, String methodName)
    throws NoSuchMethodException
  {
    this(classToCompare, methodName, DEFAULT_ORDER);
  }

  public BeanComparator(Class classToCompare, int order)
    throws NoSuchMethodException
  {
    this(classToCompare, DEFAULT_METHOD_NAME, order);
  }

  @SuppressWarnings("unchecked")
  public BeanComparator(Class classToCompare, String methodName, int order) 
    throws NoSuchMethodException
  {
    getterMethod = classToCompare.getMethod(methodName, (java.lang.Class<?>[])null);
    Class returnType = getterMethod.getReturnType();
    if ("void".equals(returnType.getName())) {
      throw new IllegalArgumentException("Cannot compare on the '"+methodName+"' method"
      + " because its return type is void (ie: it does not return a value to compare).");
    }
    if ( !doesImplement(returnType, Comparable.class.getCanonicalName()) ) {
      throw new IllegalArgumentException("Cannot compare on the '"+methodName+"' method"
      + " because its return type '"+returnType.getName()+"' does not implement Comparable.");
    }
    this.methodName = methodName;
    this.order = order;
  }

  @Override
  @SuppressWarnings("unchecked")
  public int compare(T o1, T o2) {
    try {
      Comparable o1attribute = (Comparable) getterMethod.invoke(o1, NO_ARGS);
      Comparable o2attribute = (Comparable) getterMethod.invoke(o2, NO_ARGS);
      return o1attribute.compareTo(o2attribute) * order;
    } catch (Exception e) {
      throw new IllegalStateException("Failed to compare "+clazz(o1)+ " to "+clazz(o2), e);
    }
  }

  /** 
   * Returns the type and string value of the given object.
   * eg: java.lang.String 'Hello World!'
   */
  private static String clazz(Object object) {
    return object.getClass().getCanonicalName()+" '"+String.valueOf(object)+"'";
  }

  /** 
   * Returns: Does the given clazz implement the given interfaceName 
   * @param clazz the Class to be examined.
   * @param canonicalInterfaceName the full name (with or without generics)
   *  of the interface sought: path.to.Interface[<path.to.GenericType>]
   */
  private static boolean doesImplement(Class clazz, String canonicalInterfaceName) {
    for ( Type intrface : clazz.getGenericInterfaces() ) {
      if ( intrface.toString().startsWith(canonicalInterfaceName) ) {
        return true;
      }
    }
    return false;
  }

}