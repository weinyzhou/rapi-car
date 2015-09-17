local car=require 'car';
local carGpio={
	f={12,3,5,0},
	b={6,4,2,1},
	lf={5,3,0},
	lb={6,4,1},
	rf={12,5,0},
	rb={6,4,2},
};
local function showdiagmsg(text)
	os.execute('killall diag');
	os.execute('diag -s "'..text..'"');
end

local function carStop()
	for i,v in ipairs(carGpio.f) do
		local ret=car.write(v,1);
		if ret==nil then
			--ngx.log(ngx.ERR,"write gpio:",v," failed.");
		end
	end
	for i,v in ipairs(carGpio.b) do
		local ret=car.write(v,1);
		if ret==nil then
			--ngx.log(ngx.ERR,"write gpio:",v," failed.");
		end
	end
	return true;
end
function carStopCar()
	carInit();
	carStop();
	return true;
end

function carInit()
	car.setup();
	for i,v in ipairs(carGpio.f) do
		local ret=car.pinMode(v,1);
		if ret==nil then
			--ngx.log(ngx.ERR,"pinMode gpio:",v," failed.");
		end
	end
	for i,v in ipairs(carGpio.b) do
		local ret=car.pinMode(v,1);
		if ret==nil then
			--ngx.log(ngx.ERR,"pinMode gpio:",v," failed.");
		end
	end
	return true;
end

function carFront()
	carInit();
	carStop();
	for i,v in ipairs(carGpio.f) do
		local ret=car.write(v,0);
		
		if ret==nil then
			--ngx.log(ngx.ERR,"write gpio:",v," failed.");
		end
	end
	showdiagmsg("Running");
	return true;
end

function carBack()
	carInit();
	carStop();
	for i,v in ipairs(carGpio.b) do
		local ret=car.write(v,0);
		if ret==nil then
			--ngx.log(ngx.ERR,"write gpio:",v," failed.");
		end
	end
	showdiagmsg("BACK");
	return true;
end

function carLeft()
	carInit();
	carStop();
	for i,v in ipairs(carGpio.lf) do
		local ret=car.write(v,0);
		if ret==nil then
			--ngx.log(ngx.ERR,"write gpio:",v," failed.");
		end
	end
	showdiagmsg("LEFT-F");
	return true;
end

function carLeftback()
	carInit();
	carStop();
	for i,v in ipairs(carGpio.lb) do
		local ret=car.write(v,0);
		if ret==nil then
			--ngx.log(ngx.ERR,"write gpio:",v," failed.");
		end
	end
	showdiagmsg("LEFT-B");
	return true;
end

function carRight()
	carInit();
	carStop();
	for i,v in ipairs(carGpio.rf) do
		local ret=car.write(v,0);
		if ret==nil then
			ngx.log(ngx.ERR,"write gpio:",v," failed.");
		end
	end
	showdiagmsg("RIGHT-F");
	return true;
end

function carRightback()
	carInit();
	carStop();
	for i,v in ipairs(carGpio.rb) do
		local ret=car.write(v,0);
		if ret==nil then
			ngx.log(ngx.ERR,"write gpio:",v," failed.");
		end
	end
	showdiagmsg("RIGHT-B");
	return true;
end

function carOff()
	showdiagmsg("OFF");
	os.execute('shutdown -h now');
end

function carReboot()
	showdiagmsg(self,"REBOOT");
	os.execute('reboot');
end