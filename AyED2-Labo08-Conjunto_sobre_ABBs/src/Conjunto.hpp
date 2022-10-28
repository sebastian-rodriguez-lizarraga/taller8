#include "vector"

template <class T>
Conjunto<T>::Conjunto() {
    _raiz = nullptr;
}

template <class T>
Conjunto<T>::~Conjunto() { 
    // Completar
}

template <class T>
bool Conjunto<T>::pertenece(const T& clave) const {
    Nodo* actual = _raiz;
    bool res = false;
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

/*
template<class T>
bool Conjunto<T>::pertenece_aux(const T& clave, Conjunto<T>::Nodo a) const
*/


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
    Nodo* hijo = _raiz;
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


template<class T>
bool Conjunto<T>::esHoja(Nodo *a) {
    return (a->izq == nullptr && a->der == nullptr);
}

template<class T>
bool Conjunto<T>::unSolohijo(Nodo *a) {
    return ((a->izq != nullptr) && (a->der == nullptr)) || ((a->izq == nullptr) && (a->der != nullptr));
}

// la pre es qu etenga un solo hijo el Nodo* a
template<class T>
typename Conjunto<T>::Nodo* Conjunto<T>::hijoDeNodo(Nodo *a) {
    if(a->izq != nullptr){
        return a->izq;
    }
    else{
        return a->der;
    }
}


//es un sucesor inmediato medio trucado porque borra el nodo que cumple dicha condición.
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
    if(sucesorinmediato->valor > padre->valor){
        siguient = _raiz->der;
    }
    else{
        siguient= _raiz->izq;
    }
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



template <class T>
void Conjunto<T>::remover(const T& clave) {

    if(cardinal() == 1 && esHoja(_raiz)){
        _raiz == nullptr;
    }
    Nodo* actual = _raiz;
    Nodo* siguiente = _raiz;
    while(siguiente->valor != clave){
        if(actual->valor > clave){
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
            else{
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
            else{
                Nodo* aborrar = siguiente;
                actual->izq = hijoDeNodo(siguiente);
                delete aborrar;
            }
        }
        else{ //EL CASO DIFÍCIL QUE IMPLICA AJUSTAR EL ARBOL
            const T& valor = sucesorInmediato(actual)->valor;
            ajustar(sucesorInmediato(actual));
            actual->valor = valor;
        }

        //no actualicé el valor de la raiz

}

/*
template <class T>
const T& Conjunto<T>::siguiente(const T& clave) {
    vector<T> inorder;
    if(not(clave == maximo()) && pertenece(clave)){
        while(_raiz != nullptr){
            inorder.push_back(minimo());
            remover(minimo());
        }
        /*
        return inorder[busquedaBinIn(clave) + 1];



    }
    else{
        assert(false);
    }
}

 */


//Operación minimo:

template<class T>
const T &Conjunto<T>::minimoAux(Nodo *a) const {
    if(a->izq == nullptr){
        return a->valor;
    }
    else{
        return minimoAux(a->izq);
    }
}

template <class T>
const T& Conjunto<T>::minimo() const {
       return minimoAux(_raiz);
}


//Operación maximo con recursión
template<class T>
const T& Conjunto<T>::maximoAux(Nodo *a) const {
    if(a->der == nullptr){
        return a->valor;
    }
    else{
        return maximoAux(a->der);
    }
}

template <class T>
const T& Conjunto<T>::maximo() const {
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

