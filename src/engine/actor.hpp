class Actor {
    public:
        int x;
        int y;
        std::string_view character;
        TCOD_color_t color;

        Actor(int x, int y, std::string_view character, const TCOD_color_t &color);
        void update(tcod::Console& console);
};
