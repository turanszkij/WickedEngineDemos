--lua test script
--commands:
--	debugout			: post to debug output
--	backlog_post 		: post to backlog
--	backlog_fontsize	: change font size of the backlog
--	backlog_clear		: clear backlog

separator = function(str)
backlog_post("\n\n---------------------------------\n",str,"\n---------------------------------")
end

backlog_fontsize(-3)

-- backlog_clear()
-- 
-- backlog_post("LUA test script")
-- 
-- 
-- separator("Output-test:")
-- debugout("Called debugout function from lua!")
-- backlog_post("Written to backlog from lua!")
-- 
-- separator("Add-test:")
-- a = 1
-- b = 5
-- backlog_post("a + b = ", a + b)
-- 
-- separator("Table-test:")
-- monster = {
-- 	name = "slime",
-- 	hp = 250,
-- 	mp = 40,
-- 	atk = 10,
-- 	post = function(self)
-- 		backlog_post(self.name,"\n\tHP:",self.hp,"\n\tMP:",self.mp,"\n\tATK:",self.atk)
-- 	end,
-- }
-- monster:post()
-- 
-- separator("Loop-test:")
-- for i=0 , 10 do
-- 	backlog_post(i)
-- end

local sleep=waitSeconds

runProcess( function() 
	for i=1 , 100 do
		size = 60*rand()+5
		sprite = Sprite("images/leaf.png")
		e = ImageEffects(1000*rand()-10,-500*rand()+10,size,size)
		sprite:SetEffects(e)
		a = SpriteAnim()
		a:SetRot(0.05*rand()-0.025)
		sprite:SetAnim(a)
		main:GetActiveComponent():AddSprite(sprite)
		sleep(1)
	end
end)

-- LoadModel("../../DXProject/DXProject/levels/Fencer/", "Fencer")
-- FinishLoading()





local print = backlog_post

-- -- And a function to demo it all:
-- runProcess(function ()  
--     --print("Hello world. I will now astound you by waiting for 2 seconds.")
--     --waitSeconds(2)
--     --print("Haha! I did it!")
-- 	for i=1, 60 do
-- 		waitSeconds(1)
-- 		backlog_post(i)
-- 	end
-- end) 

-- runProcess(function()  
--     print("1: I am the first function. The second function cannot speak until I say it can.")
--     waitSeconds(2)
--     print("1: In two more seconds, I will allow it to speak.")
--     waitSeconds(2)
--     signal("ok, you can talk")
--     waitSignal("function 2 done talking")
--     print("1: First function again. I'm done now too.")
-- end)
-- 
-- runProcess(function()  
--     waitSignal("ok, you can talk")
--     print("2: Hey, I'm the second function. I like talking.")
--     waitSeconds(2)
--     print("2: I'd talk all the time, if that jerky first function would let me.")
--     waitSeconds(2)
--     print("2: I guess I'm done now though.")
--     signal("function 2 done talking")
-- end) 





