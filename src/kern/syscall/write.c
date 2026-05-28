#include "/home/os161user/os161/src/userland/include/types/size_t.h"
#include "types.h"
#include "/home/os161user/os161/src/userland/include/stdio.h"
#include <lib.h>
#include "syscall.h"



ssize_t write(int filehandle, const void *buf, size_t size){
    // 2. Cast del buffer a char* per poterlo iterare
    if(filehandle != 1 &&  filehandle != 2){
        kprintf("sys_write scrive solo su stdout");
        return -1;

    }
    const char *cp = (const char *)buf;
    size_t i;

    for (i = 0; i < size; i++) {
        // In OS161 kernel-side si usa spesso putch() o kprintf()
        // Se sei in user-side, putchar() va bene, ma occhio ai parametri
        putch(cp[i]); 
    }

    // 3. Restituiamo il numero di byte effettivamente scritti
    return (ssize_t)size;
}

ssize_t read(int filehandle, void *buf, size_t size) {
    // 1. Controllo di sicurezza sul file descriptor
    if (filehandle != 0) {
        kprintf("sys_read legge solo su stdin\n");
        return -1;
    }

    // Aggiungiamo un piccolo controllo di sicurezza sul buffer
    if (buf == NULL) {
        return -1; 
    }

    char *cp = (char *)buf;
    size_t i;

    // 2. Lettura carattere per carattere
    for (i = 0; i < size; i++) {
        char c = getch();
        cp[i] = c;

        // Se l'utente preme Invio, ci fermiamo anche se non abbiamo 
        // raggiunto il limite 'size'
        if (c == '\n' || c == '\r') {
            i++; // Incrementiamo 'i' per contare anche l'invio appena salvato
            break; // Usciamo dal ciclo in anticipo
        }
    }

    // 3. ATTENZIONE QUI: Restituiamo 'i', NON 'size'!
    // Se abbiamo letto meno caratteri di 'size' a causa dell'Invio,
    // dobbiamo dire al programma utente esattamente quanti ne abbiamo letti.
    return (ssize_t)i;
}

void
sys__exit(int status)
{
  /* get address space of current process and destroy */
  struct addrspace *as = proc_getas();
  as_destroy(as);
  /* thread exits. proc data structure will be lost */
  thread_exit();

  panic("thread_exit returned (should not happen)\n");
  (void) status; // TODO: status handling
}