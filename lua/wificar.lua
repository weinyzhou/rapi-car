local cjson=require 'cjson';
local core=require 'Core';
local http=require "resty.http";
local car=require 'car';

local TLWifiCar=class("TLWifiCar",core);
TLWifiCar._VERSION = '0.1';

TLWifiCar.__index=TLWifiCar;
TLWifiCar.gpiof={12,3,5,0};
TLWifiCar.gpiob={6,4,2,1};
TLWifiCar.gpiolf={5,3,0};
TLWifiCar.gpiolb={6,4,1};
TLWifiCar.gpiorf={12,5,0};
TLWifiCar.gpiorb={6,4,2};

local function showdiagmsg(self,text)
	os.execute('killall diag');
	os.execute('diag -s "'..text..'"');
end

function TLWifiCar.stop(self)
	for i,v in ipairs(self.gpiof) do
		local ret=car.write(v,1);
		if ret==nil then
			--ngx.log(ngx.ERR,"write gpio:",v," failed.");
		end
	end
	for i,v in ipairs(self.gpiob) do
		local ret=car.write(v,1);
		if ret==nil then
			--ngx.log(ngx.ERR,"write gpio:",v," failed.");
		end
	end
	return true;
end

function TLWifiCar.init(self)
	car.setup();
	for i,v in ipairs(self.gpiof) do
		local ret=car.pinMode(v,1);
		if ret==nil then
			--ngx.log(ngx.ERR,"pinMode gpio:",v," failed.");
		end
	end
	for i,v in ipairs(self.gpiob) do
		local ret=car.pinMode(v,1);
		if ret==nil then
			--ngx.log(ngx.ERR,"pinMode gpio:",v," failed.");
		end
	end
	return true;
end




function TLWifiCar.run(self)
	self:init();
	self:stop();
	for i,v in ipairs(self.gpiof) do
		local ret=car.write(v,0);
		
		if ret==nil then
			--ngx.log(ngx.ERR,"write gpio:",v," failed.");
		end
	end
	showdiagmsg(self,"Running");
	return true;
end

function TLWifiCar.back( self )
	self:init();
	self:stop();
	for i,v in ipairs(self.gpiob) do
		local ret=car.write(v,0);
		if ret==nil then
			--ngx.log(ngx.ERR,"write gpio:",v," failed.");
		end
	end
	showdiagmsg(self,"BACK");
	return true;
end


function TLWifiCar.left(self)
	self:init();
	self:stop();
	for i,v in ipairs(self.gpiolf) do
		local ret=car.write(v,0);
		if ret==nil then
			--ngx.log(ngx.ERR,"write gpio:",v," failed.");
		end
	end
	showdiagmsg(self,"LEFT-F");
	return true;
end

function TLWifiCar.leftback(self)
	self:init();
	self:stop();
	for i,v in ipairs(self.gpiolb) do
		local ret=car.write(v,0);
		if ret==nil then
			--ngx.log(ngx.ERR,"write gpio:",v," failed.");
		end
	end
	showdiagmsg(self,"LEFT-B");
	return true;
end

function TLWifiCar.right(self)
	self:init();
	self:stop();
	for i,v in ipairs(self.gpiorf) do
		local ret=car.write(v,0);
		if ret==nil then
			ngx.log(ngx.ERR,"write gpio:",v," failed.");
		end
	end
	showdiagmsg(self,"RIGHT-F");
	return true;
end


function TLWifiCar.rightback(self)
	self:init();
	self:stop();
	for i,v in ipairs(self.gpiorb) do
		local ret=car.write(v,0);
		if ret==nil then
			ngx.log(ngx.ERR,"write gpio:",v," failed.");
		end
	end
	showdiagmsg(self,"RIGHT-B");
	return true;
end



function TLWifiCar.shutdown(self)
	showdiagmsg(self,"OFF");
	os.execute('shutdown -h now');
	
	return true;
end

function TLWifiCar.reboot(self)
	showdiagmsg(self,"REBOOT");
	os.execute('reboot');
	
	return true;
end

return TLWifiCar;