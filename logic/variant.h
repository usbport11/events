#ifndef variantH
#define variantH

class MVariant {
private:
    class MAbstractVirt {
    public:
        virtual ~MAbstractVirt() {}
    };
    template<class T>
    class MAbstract: public MAbstractVirt {
    public:
        MAbstract(T _value) {
            value = _value;
        }
        ~MAbstract() {
        }
        T getValue() const {
            return value;
        }
        T value;
    };
    MAbstractVirt* abstract;
public:
    MVariant() {
        abstract = nullptr;
    }
    ~MVariant() {
        if(abstract) delete abstract;
    }
    template<class T>
    MVariant(T value) {
        abstract = new MAbstract<T>(value);
    }
    template<class T>
    T getValue() const {
        return dynamic_cast<MAbstract<T>*>(abstract)->getValue();
    }
    template<class T>
    void setValue(T value) {
        if(abstract) delete abstract;
        abstract = new MAbstract<T>(value);
    }
};

#endif
