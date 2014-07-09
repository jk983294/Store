package com.sample.ext4.simplebook.action;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.sample.ext4.simplebook.bean.Book;
import com.sample.ext4.simplebook.bean.BookType;
import com.sample.ext4.simplebook.biz.BookService;
import com.sample.format.util.ExtHelper;


public class BookActionExt extends DispatchAction {
	/**
	 * Logger for this class
	 */
	private static final Logger logger = Logger.getLogger(BookActionExt.class);
	
	private BookService service = new BookService();
	/*
	 * 显示书籍列表页面
	 */
	public ActionForward showBookList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		return mapping.findForward("bookList");
	}
	/*
	 * 显示书籍类型列表页面
	 */
	public ActionForward showBookTypeList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		return mapping.findForward("bookTypeList");
	}
	/*
	 * 显示说明页面
	 */
	public ActionForward showAbout(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		return mapping.findForward("about");
	}
	/*
	 * 查询书籍列表
	 */
	public ActionForward getBookList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		List<Book> books = service.getBooks(); //获取书籍列表
		String xml = ExtHelper.getXmlFromList(books);//生成书籍列表对应的xml文件
		response.setContentType("application/xml;charset=UTF-8"); //设置响应类型
		response.getWriter().write(xml);//将xml写回客户端
		return null;//不进行页面跳转
	}
	/*
	 * 查询书籍类型列表
	 */
	public ActionForward getBookTypeList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		List<BookType> bookTypes = service.getBookTypes();
		String xml = ExtHelper.getXmlFromList(bookTypes);
		response.setContentType("application/xml;charset=UTF-8");
		response.getWriter().write(xml);
		return null;
	}
	/*
	 * 新增书籍
	 */
	public ActionForward addBook(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		String bookName = request.getParameter("bookName");
		String author = request.getParameter("author");
		int bookTypeId = Integer.parseInt(request.getParameter("bookTypeId"));
		float price = Float.parseFloat(request.getParameter("price"));
		String brief = request.getParameter("brief");
		Book book = new Book();
		book.setBookName(bookName);
		book.setAuthor(author);
		book.setBookTypeId(bookTypeId);
		book.setPrice(price);
		book.setBrief(brief);
		int bookId = service.addBook(book);
		boolean isSuccess = true;
		if(bookId == -1){
			isSuccess = false;
		}
		response.setContentType("text/json;charset=UTF-8");
		response.getWriter().write("{success:"+isSuccess+",bookId:"+bookId+"}");
		return null;
	}
	/*
	 * 新增书籍类型
	 */
	public ActionForward addBookType(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		String title = request.getParameter("title");
		String detail = request.getParameter("detail");
		BookType bookType = new BookType();
		bookType.setTitle(title);
		bookType.setDetail(detail);
		int bookTypeId = service.addBookType(bookType);
		boolean isSuccess = true;
		if(bookTypeId == -1){
			isSuccess = false;
		}
		response.setContentType("text/json;charset=UTF-8");
		response.getWriter().write("{success:"+isSuccess+",bookTypeId:"+bookTypeId+"}");
		return null;
	}
	/*
	 * 修改书籍信息
	 */
	public ActionForward modifyBook(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		int bookId = Integer.parseInt(request.getParameter("id"));
		String bookName = request.getParameter("bookName");
		String author = request.getParameter("author");
		int bookTypeId = Integer.parseInt(request.getParameter("bookTypeId"));
		float price = Float.parseFloat(request.getParameter("price"));
		String brief = request.getParameter("brief");
		Book book = new Book();
		book.setId(bookId);
		book.setBookName(bookName);
		book.setAuthor(author);
		book.setBookTypeId(bookTypeId);
		book.setPrice(price);
		book.setBrief(brief);
		boolean isSuccess = service.updateBook(book);
		response.setContentType("text/json;charset=UTF-8");
		response.getWriter().write("{success:"+isSuccess+",bookId:"+bookId+"}");
		return null;
	}
	/*
	 * 修改书籍类型信息
	 */
	public ActionForward modifyBookType(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		logger.debug(">>>modifyBookType");
		int bookTypeId = Integer.parseInt(request.getParameter("id"));
		String title = request.getParameter("title");
		String detail = request.getParameter("detail");
		BookType bookType = new BookType();
		bookType.setId(bookTypeId);
		bookType.setTitle(title);
		bookType.setDetail(detail);
		boolean isSuccess = service.updateBookType(bookType);
		response.setContentType("text/json;charset=UTF-8");
		String result = "{success:"+isSuccess+",bookTypeId:"+bookTypeId+"}";
		logger.debug("result="+result);
		response.getWriter().write(result);
		logger.debug("<<<modifyBookType");
		return null;
	}
	/*
	 * 删除书籍类型
	 */
	public ActionForward deleteBookType(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		int bookTypeId = Integer.parseInt(request.getParameter("bookTypeId"));
		int num = service.getBookCount(bookTypeId);
		response.setContentType("text/json;charset=UTF-8");
		if(num == 0){
			boolean isSuccess = service.deleteBookType(bookTypeId);
			response.getWriter().write("{success:"+isSuccess+",num:"+num+"}");
		}else{
			response.getWriter().write("{success:false,num:"+num+"}");
		}
		return null;
	}
	/*
	 * 根据书籍id查询书籍信息
	 */
	public ActionForward getBookById(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		response.setContentType("text/json;charset=UTF-8");
		int bookId = Integer.parseInt(request.getParameter("bookId"));
		Book book = service.getBook(bookId);
		String json = null;
		if(book != null){
			json = "{success:true,data:"+ExtHelper.getJsonFromBean(book)+"}";
		}else{
			json = "{success:false}";
		}
		System.out.println(json);
		response.getWriter().write(json);
		return null;
	}
	/*
	 * 根据书籍类型id查询书籍类型信息
	 */
	public ActionForward getBookTypeById(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		response.setContentType("text/json;charset=UTF-8");
		int bookTypeId = Integer.parseInt(request.getParameter("bookTypeId"));
		BookType bookType = service.getBookType(bookTypeId);
		String json = null;
		if(bookType != null){
			json = "{success:true,data:"+ExtHelper.getJsonFromBean(bookType)+"}";
		}else{
			json = "{success:false}";
		}
		response.getWriter().write(json);
		return null;
	}
	/*
	 * 删除书籍信息
	 */
	public ActionForward deleteBooks(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		String bookIds = request.getParameter("bookIds");
		String[] ids =  bookIds.split(",");
		List<Book> books = new ArrayList<Book>();
		for(int i = 0 ; i < ids.length ; i++){
			Book book = new Book();
			book.setId(Integer.parseInt(ids[i]));
			books.add(book);
		}
		boolean isSuccess = service.deleteBooks(books);
		response.setContentType("text/json;charset=UTF-8");
		response.getWriter().write("{success:"+isSuccess+"}");
		return null;
	}
}
