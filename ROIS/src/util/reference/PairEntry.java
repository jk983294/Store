package util.reference;

/**
 * <p>Title: XML����</p>
 *
 * <p>Description: ��ֵ��ʵ�����</p>
 *
 * <p>Copyright: Copyright (c) 2006</p>
 *
 * <p>Company: Newland</p>
 *
 * @author ZhengYanNan
 * @version 1.0
 */
@Deprecated
public class PairEntry{
  public String name = null;
  public String value = null;

  public PairEntry(){
  }

  public PairEntry(String name, String value){
    this.name = name;
    this.value = value;
  }

  public String getName(){
    return name;
  }

  public String getValue(){
    return value;
  }

  public String toString(){
    return name + "=" + value;
  }

  public boolean equals(Object obj){
    if(obj==null)
      return false;
    if(obj instanceof PairEntry){
      PairEntry pe = (PairEntry)obj;
      return name.toString().equals(pe.toString());
    }else
      return false;
  }
}
