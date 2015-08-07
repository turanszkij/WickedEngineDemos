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

 for i=0 , 10 do
 	size = 300*rand()+100
 	sprite = Sprite("images/leaf.png")
 	e = ImageEffects(1000*rand(),-500*rand(),size,size)
 	sprite:SetEffects(e)
 	a = SpriteAnim()
 	a:SetRot(0.05*rand()-0.025)
 	sprite:SetAnim(a)
 	main:GetActiveComponent():AddSprite(sprite)
 end






