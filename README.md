```
       ::::    ::: :::::::::: ::::::::::: :::::::::: :::::::::   ::::::::    
      :+:+:   :+: :+:            :+:     :+:        :+:    :+: :+:    :+:    
     :+:+:+  +:+ +:+            +:+     +:+        +:+    +:+ +:+    +:+     
    +#+ +:+ +#+ +#++:++#       +#+     +#++:++#   +#++:++#:  +#+    +:+      
   +#+  +#+#+# +#+            +#+     +#+        +#+    +#+ +#+    +#+       
  #+#   #+#+# #+#            #+#     #+#        #+#    #+# #+#    #+#        
 ###    #### ##########     ###     ########## ###    ###  ########          
```

# System

This is not a pretentious project. This is what it is.

This is an ambitious playground kernel. Design to learn more about operating-systems, kernel design and computer-architecture.
Feel free to participate to the development on your free time. 


# Compile It

Their is a makefile, just run make. Here is a list of tools you need:
 - make
 - nasm
 - gdb
 - A C compiler that support the architecture you target, x86 is the only supported architecture actually (like gcc, if you are under macos you need a cross-compiler version of gcc)
 
 The Makefile are not so clean but it does the job done and it is easly extenable.
 
# Run It

You definitivly don't want to run it on your computeur. If you install qemu and qemue-system-i386, you can run it using the qemue rule from the makefile. (make qemue or make qemue-debug if you want the debugger to be launch)

