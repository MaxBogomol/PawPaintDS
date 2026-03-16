#pragma once

class Paint;

class Tool {
    public:
        virtual ~Tool() {} 
        
        virtual void setup(Paint& paint) {}
        virtual void update(Paint& paint) {}
        virtual void open(Paint& paint) {}
        virtual void close(Paint& paint) {}
};