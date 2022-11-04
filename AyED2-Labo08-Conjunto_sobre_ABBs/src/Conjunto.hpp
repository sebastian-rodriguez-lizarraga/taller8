#include "vector"

template <class T>
Conjunto<T>::Conjunto() {
    _raiz = nullptr;
}

template <class T>
Conjunto<T>::~Conjunto() { 
    // Completar
}

//pertenece

template <class T>
bool Conjunto<T>::pertenece(const T& clave) const {
    Nodo* actual = _raiz;
    if(actual == nullptr){
        return false;
    }
    while(actual!= nullptr){
        if(actual->valor == clave){
            return true;
        }
        else if(clave < actual->valor){
            actual = actual->izq;
        }
        else{
            actual = actual->der;
        }
    }
    return false;
}

//inserción

template <class T>
void Conjunto<T>::insertar(const T& clave) {
    // si está vació el conjunto agrego un solo elemento
    if(_raiz  == nullptr){
        _raiz = new Nodo(clave);
    }
    //si la clave pertenece al conjunto no hago nada
    else if(pertenece(clave)){
        return;
    }
    //e.o.c lo inserto al conjunto
    Nodo* actual = _raiz;
    while(actual->valor != clave){
        if(actual->valor > clave) {
            Nodo* hijo = actual->izq;
            if (hijo == nullptr) {
                actual->izq = new Nodo(clave);
                actual = actual->izq;
            }
            else{
                actual = hijo;
            }
        }
            else{
                Nodo* hijo = actual->der;
                if(hijo == nullptr){
                    actual->der = new Nodo(clave);
                    actual = actual->der;
                }
                else{
                    actual = hijo;
                }
            }
    }
return;
}


//función de REMOVER y auxiliares

template<class T>
bool Conjunto<T>::esHoja(Nodo *a) {
    return (a->izq == nullptr && a->der == nullptr);
}

template<class T>
bool Conjunto<T>::unSolohijo(Nodo *a) {
    return ((a->izq != nullptr) && (a->der == nullptr)) || ((a->izq == nullptr) && (a->der != nullptr));
}

// la pre es que tenga un solo hijo el Nodo* a
template<class T>
typename Conjunto<T>::Nodo* Conjunto<T>::hijoDeNodo(Nodo *a) {
    if(a->izq != nullptr){
        return a->izq;
    }
    else{
        return a->der;
    }
}


//DADO UN NODO, devuelve el sucesorInmediato del mismo.
template<class T>
typename Conjunto<T>::Nodo* Conjunto<T>::sucesorInmediato(Nodo *padre) {
    Nodo* actual = padre;
    Nodo* siguiente = padre->der; //por la precondición se supone que padre tiene un hijo derecho.
    //porque lo voy a usar para remover nodos con dos hijos
    if(siguiente->izq == nullptr){
        return siguiente;
    }
    else{
        while(siguiente != nullptr){
            actual = siguiente;
            siguiente = siguiente->izq;
        }
        return actual;
    }
}


//lo que hace ajustar es buscar el nodo sucesorInmediato, pasado por parametro manteniendo siempre
//un puntero al padre lo que permite redireccionar el arbol en función de si
//sucesor inmediato es hoja o no, si lo es solo lo borramos y sino el puntero que antes iba del padre
//al sucesorInmediato ahora va al hijo del sucesor que solo puede ser el derecho(para que cumpla condición de sucesorinmediato)

template<class T>
void Conjunto<T>::ajustar( Nodo *sucesorinmediato) {
    Nodo* padre = _raiz;
    Nodo* siguient = _raiz;
    while(siguient != sucesorinmediato){
        if(sucesorinmediato->valor > siguient->valor){
            padre = siguient;
            siguient = padre->der;
        }
        else{
            padre = siguient;
            siguient = padre->izq;
        }
    }
    //lo que pasa acá es que siguient sale asignado con sucesorinmediato
   //ahora tengo dos casos
   if(esHoja(siguient)){     //si es hoja el sucesor inmediato lo borro sin problemas donde corresponda.
       if(padre->izq == siguient){
           padre->izq = nullptr;
           delete siguient;
       }
       else{
           padre->der = nullptr;
           delete siguient;
       }
   }
   else{        //en este caso solo puede tener un hijo que es el derecho, sino el nodo no sería el sucesor inmediato.
       if(padre->izq == siguient){
           padre->izq = siguient->der;
           delete siguient;
       }
       else{
           padre->der = siguient->der;
           delete siguient;
       }
   }

}


//la pre condición es que la clave que me pasan pertenezca al conjunto

template <class T>
void Conjunto<T>::remover(const T& clave) {
    if(pertenece(clave)){
        if(this->cardinal() == 1 && esHoja(_raiz) && _raiz->valor == clave){
            _raiz = nullptr;
            return;
        }
        Nodo* actual = _raiz;
        Nodo* siguiente = _raiz;
        while(siguiente->valor != clave){
            if(siguiente->valor > clave){
                actual=siguiente;
                siguiente = actual->izq;
            }
            else{
                actual = siguiente;
                siguiente = actual->der;
            }
        }
        //acá actual sale siendo el nodo que está antes de siguiente que es el que queremos eliminar.
        if(esHoja(siguiente)){
            if(actual->der == siguiente){
                Nodo* aborrar = siguiente;
                actual->der = nullptr;
                delete aborrar;
            }
            else if(actual->izq == siguiente){
                Nodo* aborrar = siguiente;
                actual->izq = nullptr;
                delete aborrar;
            }
        }
        else if(unSolohijo(siguiente)){
            if(actual->der == siguiente){
                Nodo* aborrar = siguiente;
                actual->der = hijoDeNodo(siguiente);
                delete aborrar;
            }
            else if(actual->izq == siguiente){
                Nodo* aborrar = siguiente;
                actual->izq = hijoDeNodo(siguiente);
                delete aborrar;
            }
            else if(siguiente == _raiz){
                Nodo* aborrar = siguiente;
                _raiz = hijoDeNodo(_raiz);
                delete aborrar;
            }
        }
        else{ //EL CASO DIFÍCIL QUE IMPLICA AJUSTAR EL ARBOL
            const T valor = sucesorInmediato(siguiente)->valor;
            //me guardo el valor del sucesorInmeidato del que quiero borrar(siguiente)
            ajustar(sucesorInmediato(siguiente));
            siguiente->valor = valor;
        }
    }
    else{
        cout << "la clave ingresada no pertenece al conjunto";
    }

}



//dar el siguiente de un elemento del arbol pasado por parametro
//ser asume que el mismo tiene siguiente, es decir que no es el maximo
//y el cardinal > 1
/*
template<class T>
int busquedaBinIn(vector<T> a, T clave) {
    int low = 0;
    int high = a.size();
    int mitad = a.size() / 2; //LA DIVISIÓN ENTERA REDONDEA PARA ABAJO EN C++
    while (high - low != 1) {
        if (a[mitad] == clave) {
            return mitad;
        } else if (a[mitad] > clave) {
            high = mitad;
        } else {
            low = mitad;
        }
        mitad = (high + low)/2;

    }
    return low;
}
*/




// pushea los elementos del arbol sin orden
/*
template<class T>
void Conjunto<T>::elementos(Nodo* a, vector<T> &res){
    if(a != nullptr){
        res.push_back(a->valor);
        elementos(a->izq,res);
        elementos(a->der,res);
    }
    else{
        return;
    }
}
*/
/*
template<class T>
void Conjunto<T>::ordenar(vector<T> &res){
    for (int i = 0; i < res.size(); ++i) {
        T minimo = res[i];
        int indiceMin = i;
        for (int j = i; j < res.size() ; ++j) {
            if(res[j] < minimo){
                minimo = res[j];
                indiceMin = j;
            }
        }
        T aux = res[i];
        res[i] = minimo;
        res[indiceMin] = aux;
    }
    return;
}
*/
//padre de nodo, supone que le paso un nodo != de _raiz, o sea que tiene padre
template<class T>
typename Conjunto<T>::Nodo* Conjunto<T>::padreDeNodo(Nodo *a) {
    Nodo* padre = _raiz;
    Nodo* siguiente = _raiz;
    while(siguiente->valor != a->valor){
        if(padre->valor < a->valor){
            padre = siguiente;
            siguiente = padre->der;
        }
        else{
            padre = siguiente;
            siguiente = padre->izq;
        }
    }
    return padre;
}

//se asume como pre que el elemento pertenece al conjunto y tiene siguiente
template <class T>
const T& Conjunto<T>::siguiente(const T& clave) {
    if(pertenece(clave)){
        Nodo* padre = _raiz;
        Nodo* siguiente = _raiz;
        while(siguiente-> valor != clave){
            if(siguiente->valor < clave){
                padre = siguiente;
                siguiente = padre->der;
            }
            else{
                padre = siguiente;
                siguiente = padre->izq;
            }
        }
        //actual sale valiendo
        //acá siguiente sale valiendo el nodo cuya clave es la que queremos borrar
        if(siguiente->der != nullptr){
            return sucesorInmediato(siguiente)->valor;
        }
        else if(padre->izq == siguiente){
            return padre->valor;
        }
        else{
            while(padreDeNodo(padre)->izq == nullptr){
                Nodo* padre = padreDeNodo(padre);
            }
            //aca nodo padre sale siendo el padre del hijo izquierdo que debo devolver como resultado
            return padre->izq->valor;
        }

    }

}





//Operación minimo:

template<class T>
const T Conjunto<T>::minimoAux(Nodo *a) const {
    if(a->izq == nullptr){
        return a->valor;
    }
    else{
        return minimoAux(a->izq);
    }
}

template <class T>
const T Conjunto<T>::minimo() const {
    if(_raiz!= nullptr) {
        return minimoAux(_raiz);
    }
    else{
        assert(false);
    }
}


//Operación maximo con recursión
template<class T>
const T Conjunto<T>::maximoAux(Nodo *a) const {
    if(a != nullptr){
        if(a->der == nullptr){
            return a->valor;
        }
        else{
            return maximoAux(a->der);
        }
    }
    else{
        assert(false);
    }

}

template <class T>
const T Conjunto<T>::maximo() const {
    return maximoAux(_raiz);
}

// Operación cardinal resuelta recursivamente.

template<class T>
unsigned int Conjunto<T>::cardinalAux(Nodo *elem) const {
    if(elem == nullptr){
        return 0;
    }
    else{
          return 1 + cardinalAux(elem->izq) + cardinalAux(elem->der);
    }
}


template <class T>
unsigned int Conjunto<T>::cardinal() const {
    if(_raiz == nullptr){
        return 0;
    }
    else{
        return 1 + cardinalAux(_raiz->izq) + cardinalAux(_raiz->der);
    }
}

template <class T>
void Conjunto<T>::mostrar(std::ostream&) const {
    assert(false);
}

