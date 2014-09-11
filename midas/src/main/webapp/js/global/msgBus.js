msgbus = new Mediator();

//layoutController
msgbus.subscribe("showCmp", showCmp );
msgbus.subscribe("hideCmp",  hideCmp);

//index controller
msgbus.subscribe("showIndex", onClickIndex );
