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
  
  
  
-- --Screen saver
--  runProcess(function()
--	local screenW = GetScreenWidth()
--	local screenH = GetScreenHeight()
--	local velX = 1.5
--	local velY = 1.5
--	
--	local sprite = Sprite("images/blood2.png")
--	local effects = ImageEffects(posX,posY,100,100)
--	local anim = SpriteAnim()
--	sprite:SetEffects(effects)
--	sprite:SetAnim(anim)
--	main:GetActiveComponent():AddSprite(sprite)
--	local UpdateSprite = function()
--		effects:SetPos(posX,posY)
--		sprite:SetEffects(effects)
--	end
--	
--	while true do
--		if(posX < 0 or posX + 100 > screenW) then
--			velX = -velX
--		end
--		if(posY > 0 or posY - 100 < -screenH) then
--			velY = -velY
--		end
--		posX = posX + velX
--		posY = posY + velY
--		UpdateSprite()
--		sleep(0.01666)
--	end
--  end)
  
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
 LoadModel("models/girl/","girl","player")
 FinishLoading()
end)

local DrawAxis = function(point,f)
	DrawLine(point,point:Add(Vector(f,0,0)),Vector(1,0,0,0))
	DrawLine(point,point:Add(Vector(0,f,0)),Vector(0,1,0,0))
	DrawLine(point,point:Add(Vector(0,0,f)),Vector(0,0,1,0))
end
local DrawAxisTransformed = function(point,f,transform)
	DrawLine(point:Transform(transform),point:Add(Vector(f,0,0)):Transform(transform),Vector(1,0,0,0))
	DrawLine(point:Transform(transform),point:Add(Vector(0,f,0)):Transform(transform),Vector(0,1,0,0))
	DrawLine(point:Transform(transform),point:Add(Vector(0,0,f)):Transform(transform),Vector(0,0,1,0))
end

girl = GetArmature("Armature_player")
head = girl:GetBone("testa")
local p,n = Vector(0,0,0)
local face = Vector(0,0,1)
local velocity = Vector()
local ray = Ray()
camera = GetCamera()
gravity = Vector(0,-0.076,0)

runProcess(function()

	girl:ClearTransform()
	girl:Rotate(Vector(0,3.1415))
	girl:Scale(Vector(1.9,1.9,1.9))
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
		local velocityPrev = velocity;
		velocity = face:Multiply(Vector(f,f,f))
		velocity:SetY(velocityPrev:GetY())
		ray = Ray(girl:GetPosition():Add(velocity):Add(Vector(0,4)),Vector(0,-1,0))
		o,p,n = Pick(ray)
		if(o:IsValid()) then
			state = states.WALK
		else
			state = states.STAND
			velocity = velocityPrev
		end
		--front block
		local ray2 = Ray(girl:GetPosition():Add(Vector(0,4)),girl:GetPosition():Add(velocity):Add(Vector(0,4)))
		local o2,p2,n2 = Pick(ray)
		local dist = vector.Subtract(girl:GetPosition():Add(Vector(0,4)),p2):Length()
		if(o2:IsValid() and o2:GetName() ~= "omino_player" and dist < 3.3) then
			print(dist)
			state = states.STAND
			velocity = velocityPrev
		end
	end
	function Turn(f)
		girl:Rotate(Vector(0,f))
		face = face:Transform(matrix.RotationY(f))
		state = states.TURN
	end
	function Jump(f)
		velocity=velocity:Add(Vector(0,f,0))
		state = states.JUMP
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
				MoveForward(0.25)
			end
			if(input.Down(VK_DOWN)) then
				MoveForward(-0.25)
			end
		
		end
		
		if( input.Press(string.byte('J'))) then
			Jump(1.3)
		end
		
		
		
		if(state == states.STAND) then
			girl:PauseAction()
			state = states.STAND
		elseif(state == states.TURN) then
			girl:PlayAction()
			state = states.STAND
		elseif(state == states.WALK) then
			girl:PlayAction()
			state = states.STAND
		elseif(state == states.JUMP) then
			girl:PauseAction()
			state = states.STAND
		end
		
		w,wp,wn = Pick(ray,PICK_WATER)
		if(w:IsValid() and velocity:Length()>0) then
			PutWaterRipple("images/ripple.png",wp)
		end
		
		
		velocity = velocity:Add(gravity)
		girl:Translate(velocity)
		ray = Ray(girl:GetPosition():Add(Vector(0,4)),Vector(0,-1,0))
		local pPrev = p
		o,p,n = Pick(ray)
		if(not o:IsValid()) then
			p=pPrev
		end
		if(girl:GetPosition().GetY() < p.GetY() and velocity:GetY()<=0) then
			state = states.STAND
			girl:Translate(vector.Subtract(p,girl:GetPosition()))
			velocity=Vector()
		end
		
		
		
		
		update()
	end
end)


runProcess(function()
	while true do
		
		--velocity
		DrawLine(girl:GetPosition():Add(Vector(0,4)),girl:GetPosition():Add(Vector(0,4)):Add(velocity))
		--face
		DrawLine(girl:GetPosition():Add(Vector(0,4)),girl:GetPosition():Add(Vector(0,4)):Add(face:Normalize()),Vector(1,0,0,1))
		--intersection
		DrawAxis(p,0.5)
		--head
		--DrawAxisTransformed(Vector(),0.6,head:GetMatrix())
		--head:GetMatrix()
		--DrawAxis(head:GetPosition(),0.6)
		
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

-- -- Font
-- font = Font("ASSADSDFASMLKFjkdfkjIIKSKNmmm kaskdjjiopasdjk kkasdjio")
-- main:GetActiveComponent():AddFont(font)
