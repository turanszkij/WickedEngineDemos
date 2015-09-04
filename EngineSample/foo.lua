--lua test script
--commands:
--	debugout			: post to debug output
--	backlog_post 		: post to backlog
--	backlog_fontsize	: change font size of the backlog
--	backlog_clear		: clear backlog

local print = backlog_post

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

-- runProcess( function() 
-- 	for i=1 , 100 do
-- 		size = 60*rand()+5
-- 		sprite = Sprite("images/leaf.png")
-- 		e = ImageEffects(GetScreenWidth()*rand()-10,-GetScreenHeight()*rand()+10,size,size)
-- 		sprite:SetEffects(e)
-- 		a = SpriteAnim()
-- 		a:SetRot(0.05*rand()-0.025)
-- 		sprite:SetAnim(a)
-- 		main:GetActiveComponent():AddSprite(sprite)
-- 		sleep(1)
-- 	end
-- end)

-- runProcess(function()
--  LoadModel("../../DXProject/DXProject/levels/Training Ground/", "Training Ground")
--  FinishLoading()
-- end)
-- w = GetTransform("wood_barrel_common")
-- g=GetTransform("Ground_common")
-- m=MatrixTranslation(Vector(2))

-- runProcess(function()
-- 	local lopatky = GetTransform("lopatky_common")
-- 	local mlyn = GetTransform("mlyn_common")
-- 	lopatky:AttachTo(mlyn)
-- 	local v = Vector(0,0,0,0)
-- 	while true do
-- 		sleep(0.01)
-- 		v:SetX(v:GetX()-0.001)
-- 		mlyn:Translate(v)
-- 	end
-- end)


-- if(w:Intersects(g)) then
-- 	print("YES!")
-- else 
-- 	print("NO")
-- end



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

-- backlog_post_list = function(a)
-- 	for key,value in pairs(a) do
-- 		backlog_post(value)
-- 	end
-- end
-- 
-- function len(T)
--   local count = 0
--   for _ in pairs(T) do 
-- 	count = count + 1 end
--   return count
-- end
-- 
-- backlog_post(len({GetObjects()}))
-- backlog_post_list({GetObjects()})

-- local up=0xC8
-- 
--  runProcess(function()
--  	while true do
--  		waitSignal("upkey")
--  		SoundEffect("sound/change.wav"):Play()
--  	end
--  end)
-- 

  local posX = GetScreenWidth()/2
  local posY = -GetScreenHeight()/2
  local stepX = posX*0.1
  local stepY = posY*0.1
--  AddSprite = function()
--  	local sprite = Sprite("images/blood2.png")
--  	local effects = ImageEffects(posX,posY,100,100)
--  	local anim = SpriteAnim()
--  	sprite:SetEffects(effects)
--  	sprite:SetAnim(anim)
--  	main:GetActiveComponent():AddSprite(sprite)
--  end
  --AddSprite()
  
  
  
 --Screen saver
  runProcess(function()
	local screenW = GetScreenWidth()
	local screenH = GetScreenHeight()
	local velX = 1.5
	local velY = 1.5
	
	local sprite = Sprite("images/blood2.png")
	local effects = ImageEffects(posX,posY,100,100)
	local anim = SpriteAnim()
	sprite:SetEffects(effects)
	sprite:SetAnim(anim)
	main:GetActiveComponent():AddSprite(sprite)
	local UpdateSprite = function()
		effects:SetPos(posX,posY)
		sprite:SetEffects(effects)
	end
	
	while true do
		if(posX < 0 or posX + 100 > screenW) then
			velX = -velX
		end
		if(posY > 0 or posY - 100 < -screenH) then
			velY = -velY
		end
		posX = posX + velX
		posY = posY + velY
		UpdateSprite()
		sleep(0.01666)
	end
  end)
  
--  
-- 
-- 
-- runProcess(function()
-- 	while true do
-- 		waitSignal("upkey")
--  		SoundEffect("sound/change.wav"):Play()
-- 		posY = posY - stepY;
-- 		UpdateSprite()
-- 		--AddSprite()
-- 	end
-- end)
-- runProcess(function()
-- 	while true do
-- 		waitSignal("downkey")
--  		SoundEffect("sound/change.wav"):Play()
-- 		posY = posY + stepY;
-- 		UpdateSprite()
-- 		--AddSprite()
-- 	end
-- end)
-- runProcess(function()
-- 	while true do
-- 		waitSignal("leftkey")
--  		SoundEffect("sound/change.wav"):Play()
-- 		posX = posX + stepY;
-- 		UpdateSprite()
-- 		--AddSprite()
-- 	end
-- end)
-- runProcess(function()
-- 	while true do
-- 		waitSignal("rightkey")
--  		SoundEffect("sound/change.wav"):Play()
-- 		posX = posX - stepY;
-- 		UpdateSprite()
-- 		--AddSprite()
-- 	end
-- end)

-- o=GetObject("wood_barrel_common")
-- o:Translate(Vector(1))


-- Control Sample
runProcess(function()
 --LoadModel("../game/levels/Training Ground/", "Training Ground","common",MatrixTranslation(Vector(0,4)))
 --LoadModel("../../DXProject/DXProject/levels/Training Ground/", "Training Ground")
 LoadModel("models/girl/","girl")
 FinishLoading()
end)

local DrawAxis = function(point,f)
	DrawLine(point,point:Add(Vector(f,0,0)),Vector(1,0,0,0))
	DrawLine(point,point:Add(Vector(0,f,0)),Vector(0,1,0,0))
	DrawLine(point,point:Add(Vector(0,0,f)),Vector(0,0,1,0))
end

local raypos = Vector(0,2,0)
local girl = GetArmature("Armature_common")
local realPos = Vector(0,0,0)
local p,n = Vector(0,0,0)
local face = Vector(0,0,1)
local velocity = Vector()
local ray = Ray()
camera = GetCamera()

runProcess(function()

	girl:ClearTransform()
	girl:Translate(Vector(0,4,0))
	girl:Rotate(Vector(0,3.1415))
	girl:Scale(Vector(2,2,2))
	camera:ClearTransform()
	camera:Rotate(Vector(0.1))
	camera:Translate(Vector(2,7,-12))
	camera:AttachTo(girl)
	
	states = {
		STAND = 0,
		TURN = 1,
		WALK = 2,
		JUMP = 3,
	}
	local state = states.STAND
	
	function MoveForward(f)
		velocity = face:Multiply(Vector(f,f,f))
		raypos = girl:GetPosition():Add(velocity)
		ray = Ray(raypos,Vector(0,-1,0))
		o,p,n = Pick(ray)
		if(o:IsValid()) then
			state = states.WALK
		else
			raypos = vector.Subtract(raypos,velocity)
			state = states.STAND
		end
	end
	function Turn(f)
		girl:Rotate(Vector(0,f))
		face:Transform(matrix.RotationY(f))
		state = states.TURN
	end
	
	while true do
		
		while( backlog_isactive() ) do
			sleep(1)
		end
		
		if(input.Press(string.byte('R'))) then
			camera:Detach()
			camera:ClearTransform()
		end
		if(input.Press(string.byte('C'))) then
			camera:AttachTo(girl)
		end
		
		if(state==states.STAND) then
			if(input.Down(VK_LEFT)) then
				Turn(-0.08)
			end
			if(input.Down(VK_RIGHT)) then
				Turn(0.08)
			end
		
			if(input.Down(VK_UP)) then
				MoveForward(0.15)
			end
			if(input.Down(VK_DOWN)) then
				MoveForward(-0.15)
			end
		
		end
		
		if( not (state == states.JUMP) and input.Press(string.byte('J'))) then
			state = states.JUMP
			velocity=velocity:Multiply(Vector(2,2,2))
			velocity=velocity:Add(Vector(0,1,0))
		end
		
		
		
		if(state == states.STAND) then
			girl:PauseAction()
			velocity=Vector()
			velocity = Vector()
		elseif(state == states.TURN) then
			girl:PlayAction()
			velocity=Vector()
			state = states.STAND
		elseif(state == states.WALK) then
			girl:PlayAction()
			girl:Translate(vector.Subtract(p,realPos))
			realPos = p
			state = states.STAND
		elseif(state == states.JUMP) then
			girl:PauseAction()
			girl:Translate(velocity)
			raypos = raypos:Add(velocity)
			realPos = realPos:Add(velocity)
			velocity = velocity:Subtract(Vector(0,0.076,0))
			
			ray = Ray(raypos,Vector(0,-1,0))
			o,p,n = Pick(ray)
			
			if(realPos.GetY() < p.GetY()) then
				state = states.STAND
				girl:Translate(vector.Subtract(p,realPos))
				realPos = p
			end
			
		end
		
		w,wp,wn = Pick(ray,PICK_WATER)
		if(w:IsValid() and velocity:Length()>0) then
			PutWaterRipple("images/ripple.png",wp)
		end
		
		
		update()
	end
end)


runProcess(function()
	while true do
		
		DrawLine(raypos,raypos:Add(Vector(0,-1)))
		DrawLine(girl:GetPosition(),girl:GetPosition():Add(face:Normalize()),Vector(1,0,0,1))
		DrawAxis(p,0.5)
		--DrawLine(girl:GetPosition(),girl:GetPosition():Add(Vector(0,-1)))
		
		render()
	end
end)

--Picking
-- a=Vector(10,10,10)
-- b=Vector(0,-1,0)
-- r=Ray(a,b)
-- object,pos,nor=Pick(r)
-- separator("OBJECT:")
-- print(object:GetName())
-- separator("POS")
-- backlog_post(pos.GetX(),", ",pos.GetY(),", ",pos.GetZ())
-- separator("NOR")
-- backlog_post(nor.GetX(),", ",nor.GetY(),", ",nor.GetZ())


-- -- input
-- runProcess(function()
-- 	while true do
-- 		if(input.Down(string.byte('W'))) then
-- 			print("ALLAT")
-- 		end
-- 		tick()
-- 	end
-- end)

-- Font
font = Font("ASSADSDFASMLKFjkdfkjIIKSKNmmm kaskdjjiopasdjk kkasdjio")
main:GetActiveComponent():AddFont(font)
