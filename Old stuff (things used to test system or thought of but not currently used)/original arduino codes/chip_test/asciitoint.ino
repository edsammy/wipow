int asciitoint(int c){
        if(c<58&&c>47) { // 0-9
          return (c-48);
        }
        else if(c<71&&c>64) { // A-F
          return (c-55);
        }
        else if(c<103&&c>96) { // a-f
          return (c-87);
        }
}
