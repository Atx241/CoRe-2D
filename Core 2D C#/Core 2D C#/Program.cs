namespace Core
{
    internal class Program
    {
        static Entity entity1;
        static Entity entity2;
        static AnchoredEntity aEntity1;
        static double deltaTime;
        private static DateTime previousTime;
        static void Main(string[] args)
        {
            Utils.Init(new GameFunc(Start), new GameFunc(Update), new Vector3(0.2, 0.2, 0.6));
        }
        static void Start()
        {
            previousTime = DateTime.Now.AddSeconds(-0.016);
            Texture t1 = Texture.CreateTexture([0xFF,0xFF,0xFF,128], 1, 1);
            Texture t2 = Texture.CreateTexture("ball.png");
            entity1 = Entity.CreateRenderEntity(new Entity(t1, new Vector2(0, 0), 0, new Vector2(2, 0.5),1));
            entity2 = Entity.CreateRenderEntity(new Entity(entity1, t2, new Vector2(0.5, 0), 90, new Vector2(1, 1), 0));
            //aEntity1 = AnchoredEntity.CreateRenderEntity(new AnchoredEntity(new Vector2(-1,-1),t2,new Vector2(0.5,0.5),0,new Vector2(1,1),2,true));
            Camera.cameraPosition = new Vector2(0, 0);
            Camera.cameraZoom = 1;
            Input.onKeyPress += (int key, int action) => {
                if (action == 1)
                {
                    switch (key)
                    {
                    case 65: Camera.cameraPosition.x -= 1; break;
                    case 68: Camera.cameraPosition.x += 1; break;
                    case 83: Camera.cameraPosition.y -= 1; break;
                    case 87: Camera.cameraPosition.y += 1; break;
                    }
                }
            };
        }
        static void Update()
        {
            deltaTime = (DateTime.Now - previousTime).TotalSeconds;
            previousTime = DateTime.Now;
            entity1.rotation += (float)(90 * deltaTime);
            //entity2.rotation += (float)(90 * deltaTime);
        }
    }
}
