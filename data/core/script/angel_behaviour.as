/*
 * Copyright (c) 2023.
 */

shared class AngelBehaviour {
    AngelBehaviour() {
        @self = AngelBehaviour_t();
    }

    void start() {}

    void update() {}

    void tick() {}

    void finish() {}

    GameObject @get_gameObject() property {
        return self.getGameObject();
    }

    private AngelBehaviour_t @self;
}