package com.sample.ext4.simplebook.action;

import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.sample.ext4.simplebook.bean.Book;
import com.sample.ext4.simplebook.bean.BookType;
import com.sample.ext4.simplebook.biz.BookService;

/**
 * 简易图书管理系统Action类
 */
public class BookAction extends DispatchAction{
	//创建业务层对象。跳转到书籍类型列表页面
	private BookService service = new BookService();
	
	/**
	 * 跳转到书籍类型列表页面
	 */
	public ActionForward showBookTypeList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		List<BookType> bookTypes = service.getBookTypes();//获取书籍类型列表
		request.setAttribute("bookTypes", bookTypes);//将书籍类型列表放到请求中
		return mapping.findForward("bookTypeList");//跳转到书籍类型列表页面
	}
	
	/**
	 * 跳转到书籍列表页面
	 */
	public ActionForward showBookList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		List<Book> books = service.getBooks();//获取书籍列表
		request.setAttribute("books", books);//将书籍列表放到请求中
		return mapping.findForward("bookList"); //跳转到书籍列表页面
	}
	
	/**
	 * 跳转到书籍新增页面
	 */
	public ActionForward toAddBook(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		List<BookType> bookTypes = service.getBookTypes(); //获取书籍类型列表
		request.setAttribute("bookTypes", bookTypes);//将书籍类型列表放到请求中
		return mapping.findForward("addBook");//跳转到书籍新增页面
	}
	
	/**
	 * 跳转到书籍修改页面
	 */
	public ActionForward toModifyBook(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		int bookId = Integer.parseInt(request.getParameter("bookId"));//获取书籍id
		List<BookType> bookTypes = service.getBookTypes();//获取书籍类型列表
		request.setAttribute("bookTypes", bookTypes);//将书籍类型列表放到请求中
		Book book = service.getBook(bookId); //获取指定id的书籍信息
		request.setAttribute("book", book);//将书籍信息放到请求中
		return mapping.findForward("modifyBook");//跳转到书籍修改页面
	}
	
	/**
	 * 修改书籍信息
	 */
	public ActionForward modifyBook(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		int bookId = Integer.parseInt(request.getParameter("bookId"));//获取书籍id
		String bookName = request.getParameter("bookName");//获取书籍名称
		String author = request.getParameter("author");//获取书籍作者
		int bookTypeId = Integer.parseInt(request.getParameter("bookTypeId")); //获取类型id
		float price = Float.parseFloat(request.getParameter("price"));//获取书籍金额
		String brief = request.getParameter("brief");//获取书籍简介
		Book book = new Book(); //创建书籍对象
		book.setId(bookId);//设置书籍对象id；
		book.setBookName(bookName);//设置书籍对象名称信息
		book.setAuthor(author);//设置书籍对象作者信息
		book.setBookTypeId(bookTypeId);//设置书籍对象类型id信息
		book.setPrice(price);//设置书籍对象金额信息；
		book.setBrief(brief);//设置书籍对象简介信息
		boolean isSuccess = service.updateBook(book);//更新书籍信息
		if(isSuccess){//判断更新书籍信息是否成功
			return mapping.findForward("toBookList");//更新成功跳转到书籍列表页面
		}else{
			return mapping.findForward("error");//更新失败跳转到错误页面
		}
	}
	
	/**
	 * 新增书籍
	 * 操作成功后将页面跳转到书籍列表页面
	 */
	public ActionForward addBook(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		String bookName = request.getParameter("bookName");//获取书籍名称
		String author = request.getParameter("author");//获取书籍作者
		int bookTypeId = Integer.parseInt(request.getParameter("bookTypeId"));//获取类型id
		float price = Float.parseFloat(request.getParameter("price"));//获取书籍金额
		String brief = request.getParameter("brief");//获取书籍简介
		Book book = new Book();//创建书籍对象；
		book.setBookName(bookName);//设置书籍对象名称信息
		book.setAuthor(author);//设置书籍对象作者信息
		book.setBookTypeId(bookTypeId);//设置书籍对象类型id信息
		book.setPrice(price);//设置书籍对象金额信息
		book.setBrief(brief);//设置书籍对象简介信息
		int bookId = service.addBook(book);//新增书籍信息
		if(bookId != -1){//判断新增书籍信息是否成功
			return mapping.findForward("toBookList");//新增成功跳转到书籍列表页面
		}else{
			return mapping.findForward("error"); //新增失败跳转到错误页面
		}
	}
	
	/**
	 * 新增书籍类型
	 * 操作成功后将页面跳转到书籍类型列表页面
	 */
	public ActionForward addBookType(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		String title = request.getParameter("title"); //获取书籍类型标题
		String detail = request.getParameter("detail");//获取书籍类型描述
		BookType bookType = new BookType();//创建书籍类型对象
		bookType.setTitle(title);//设置书籍类型对象标题信息
		bookType.setDetail(detail);//设置书籍类型对象描述信息
		int bookTypeId = service.addBookType(bookType);//新增书籍类型
		if(bookTypeId != -1){ //判断书籍类型新增是否成功
			return mapping.findForward("toBookTypeList");//新增成功则跳转到类型列表
		}else{
			return mapping.findForward("error"); //新增失败则跳转到错误页面
		}
	}
	
	/**
	 * 跳转到书籍类型新增页面
	 */
	public ActionForward toAddBookType(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		return mapping.findForward("addBookType");//跳转到书籍类型列表页面
	}
	
	/**
	 * 跳转到书籍类型修改页面
	 */
	public ActionForward toModifyBookType(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		int bookTypeId = Integer.parseInt(request.getParameter("bookTypeId"));//获取类型id
		BookType bookType = service.getBookType(bookTypeId);//获取指定id的书籍类型
		request.setAttribute("bookType", bookType);//将书籍类型放入请求中
		return mapping.findForward("modifyBookType");//跳转到书籍类型修改页面
	}
	
	/**
	 * 修改书籍类型信息
	 */
	public ActionForward modifyBookType(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		int bookTypeId = Integer.parseInt(request.getParameter("bookTypeId"));//获取类型id
		String title = request.getParameter("title");//获取类型标题
		String detail = request.getParameter("detail");//获取类型说明
		BookType bookType = new BookType();//创建书籍类型对象
		bookType.setId(bookTypeId);//设置书籍类型对象的id信息；
		bookType.setTitle(title);//设置书籍类型对象的标题信息
		bookType.setDetail(detail);//设置书籍类型对象的描述信息
		boolean isSuccess = service.updateBookType(bookType);//更新书籍类型信息
		if(isSuccess){ //判断更新是否成功
			return mapping.findForward("toBookTypeList"); //成功则跳转到列表页面
		}else{
			return mapping.findForward("error");//失败则跳转到错误页面
		}
	}
	
	/**
	 * 删除书籍类型
	 * 操作成功后将页面跳转到书籍类型列表页面
	 */
	public ActionForward deleteBookType(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		int bookTypeId = Integer.parseInt(request.getParameter("bookTypeId"));//获取类型id
		int count = service.getBookCount(bookTypeId);//获取指定书籍类型id的书籍数量
		if(count == 0){//判断书籍是否为0；书籍数量为0则可以删除书籍类型
			boolean isSuccess = service.deleteBookType(bookTypeId); //删除指定id的书籍类型
			if(isSuccess){ //判断删除是否成功
				return mapping.findForward("toBookTypeList");//成功则跳转到类型列表
			}else{
				return mapping.findForward("error"); //失败则跳转到错误页面
			}
		}else{
			//书籍数量不为0则不可以删除书籍类型
			request.setAttribute("message", "已包含书籍的书籍类型不能删除！");
			request.setAttribute("method", "showBookTypeList");
			return mapping.findForward("message");
		}
	}
	
	/**
	 * 删除书籍
	 * 操作成功后将页面跳转到书籍列表页面
	 */
	public ActionForward deleteBook(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		int bookId = Integer.parseInt(request.getParameter("bookId"));//获取书籍id
		Book book = new Book();//创建书籍对象
		book.setId(bookId);//设置书籍id
		boolean isSuccess = service.deleteBook(book);//删除指定id的书籍信息
		if(isSuccess){ //判断书籍删除是否成功
			return mapping.findForward("toBookList"); //成功则跳转到书籍列表页面
		}else{
			return mapping.findForward("error"); //失败则跳转到错误页面
		}
	}
}