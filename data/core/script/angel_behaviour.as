/*
 * Copyright (c) 2023.
 */

shared interface IAngelBehaviour {
    void start();
    void update();
    void finish();
}

shared class AngelBehaviour : IAngelBehaviour {
    void start() {}
    void update() {}
    void finish() {}

    protected GameObject getGameObject() {
        // TODO
    }
}