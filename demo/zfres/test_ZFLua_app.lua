local pm = ZFLuaExecuteLocalFile("test_ZFLua_app_PageManager.lua");

local window = pm:embededCreateWindow();
window:windowShow();
pm:managerContainer():layoutParam():layoutMarginSet(ZFUIMarginMake(20));

ZFLuaExecuteLocalFile("test_ZFLua_app_Page.lua", ZFCoreArrayCreate(pm));

return window;

