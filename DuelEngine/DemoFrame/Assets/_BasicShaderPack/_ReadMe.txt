这里是基本shader包的目录,
注意为了达到通用性, shader统一使用vs/ps后缀名,而不用glsl的vert/frag后缀名规范, 而且不使用资源包中的前缀命名规范
在不同的rendersystem里加载资源的时候要注意使用不同的Desc, 如BasicShaderGLSL(Opengl),BasicShaderHLSL(DX), 除了DRE可以通用.
在GroupDescription中也要作出相应更改, 使得改变_BasicShaderPack中的指向目录.