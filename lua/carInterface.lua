local cjson=require 'cjson';
local wificar=require 'wificar';

local pObj=wificar:new();

local post_type=pObj:check_is_post();
ngx.header.content_type="application/json; charset=utf-8";

if post_type then
	pObj:print_Err();
	return ;
else
	args=ngx.req.get_uri_args();
end

local url=ngx.var.request_uri;
local flag=string.find(url,"?");
local path=url;
if flag then
	path=string.sub(url,1,flag-1);
end

local arr=string.split(path,'/');

if table.nums(arr)~=3 then
	pObj:print_Err();
	--ngx.say(table.nums(arr));
	return ;
end

local oper=arr[table.nums(arr)];


if oper=="setup" then
	pObj:init();
	pObj:stop();
	ngx.say('{"status":200,"msg":"链接测试成功"}');
elseif oper=="run" then
	pObj:run();
	ngx.say('{"status":200,"msg":"前进中"}');
elseif oper=="back" then
	pObj:back();
	ngx.say('{"status":200,"msg":"倒车中"}');
elseif oper=="left" then
	pObj:left();
	ngx.say('{"status":200,"msg":"左转中"}');
elseif oper=="leftback" then
	pObj:leftback();
	ngx.say('{"status":200,"msg":"左后转中"}');
elseif oper=="right" then
	pObj:right();
	ngx.say('{"status":200,"msg":"右转中"}');
elseif oper=="rightback" then
	pObj:rightback();
	ngx.say('{"status":200,"msg":"右后转中"}');
elseif oper=="stop" then
	pObj:stop();
	ngx.say('{"status":200,"msg":"已经停止"}');
elseif oper=="shutdown" then
	pObj:shutdown();
	ngx.say('{"status":200,"msg":"正在关机中..."}');
elseif oper=="reboot" then
	pObj:reboot();
	ngx.say('{"status":200,"msg":"正在重启中..."}');
else
	ngx.say('{"status":0,"msg":"无效指令"}');
end


ngx.exit(200);