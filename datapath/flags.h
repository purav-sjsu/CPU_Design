#ifndef FLAGS_H
#define FLAGS_H

// Pointing to your existing memory folder
#include "../memory/flipflop.h" 

class FlagsRegister {
    public:
        FlagsRegister();
        
        void update(bool z, bool n, bool c, bool v);
        
        bool getZ() const;
        bool getN() const;
        bool getC() const;
        bool getV() const;

    private:
        DFlipFlop z_ff;
        DFlipFlop n_ff;
        DFlipFlop c_ff;
        DFlipFlop v_ff;
};

#endif // FLAGS_H