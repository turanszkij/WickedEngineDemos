-- s = Sprite("HelloWorldDemo/HelloWorld.png");
-- s.SetEffects(ImageEffects(100,-100,400,400));
-- f = Font("Hello World Font renderer");
-- f.SetPos(Vector(20,-150));

-- runProcess(function()
-- 	local isserver = false
-- 	local isclient = false
-- 	while true do
-- 		
-- 		if(input.Press(string.byte('S'))) then
-- 			isserver = true
-- 			server = Server()
-- 		end
-- 		if(input.Press(string.byte('C'))) then
-- 			isclient = true
-- 			client = Client()
-- 		end
-- 		
-- 		if(isserver) then
-- 			server.Poll()
-- 		end
-- 		if(isclient) then
-- 			client.Poll()
-- 		end
-- 		
-- 		update()
-- 	end
-- end)

runProcess(function()
	while true do
		
		if(input.Press(VK_F12)) then
			ReloadShaders("../../WickedEngine/WickedEngine/shaders/")
		end
		
		if(input.Press(VK_F11)) then
			PutEnvProbe(GetCamera().GetPosition(),256);
		end
		
		-- if(input.Press(VK_F6)) then
			-- main.GetActiveComponent().SetDepthOfFieldEnabled(true);
		-- end
		
		update()
	end
end)