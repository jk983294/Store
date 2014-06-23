#pragma once

#define DATA_OPERATION_MENU_DEF(cn) typedef void (cn::*Action)(CData data); \
							static Action OperationMenu[MAXOPERATIONUM]; \
							enum Directions; \
							virtual void OpDispatch(int opid , CData data) \
							{ \
								(this->*OperationMenu[opid])(data); \
							}

#define OPIMPNAMEDEF_BEGIN(cn)  cn::Action cn::OperationMenu[] = {
#define OPIMPNAMEDEF_END };
#define IMPORT_OPIMPNAMEDEF(cn)  extern cn::Action cn::OperationMenu[]

#define OPIMPIDDEF_BEGIN(cn) enum cn::Directions {
#define OPIMPIDDEF_END };
#define IMPORT_OPIMPIDDEF_BEGIN(cn) extern enum cn::Directions {
#define IMPORT_OPIMPIDDEF_END };

#define OPNAMEDEF(cn) &cn::
#define OPIDDEF(cn) cn::

#define OPID(op) _##op


class DLL_API CDataHandler
{
public:
	CDataHandler(void);
	~CDataHandler(void);

	virtual void OpDispatch(int opid , CData data) = 0;
};
