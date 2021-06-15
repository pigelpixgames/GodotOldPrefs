# GodotOldPrefs
GodotOldPrefs is a module for Godot Engine 2.1 and 3.x that lets you read saved Unity PlayerPrefs data from within Godot Engine on Windows, Mac and Linux.

## Example

```gdscript
func _ready():
	var prefstest = GodotOldPrefs.new()
	prefstest.setCredentials("DefaultCompany", "New Unity Project")
	print(prefstest.getFloat("TestFloat"))
	print(prefstest.getInt("TestInt"))
	print(prefstest.getString("TestString"))
	print(prefstest.getBool("TestBool"))
	print(prefstest.keyExists("TestKey"))
```


## Methods

#### setCredentials(CompanyName, ProductName)
Set credentials, as you've set them in Unity.

#### getInt(IntName)
Get integral pref value.

#### getFloat(FloatName) `On Mac:` getFloatAsString(FloatName)
Get floating point pref value.

#### getString(StringName)
Get string pref value.

#### getBool(BoolName)
Get bool pref value.

#### keyExists(KeyName)
Check if pref exists.


## Building
* Navigate to [releases](https://github.com/pigelpixgames/GodotOldPrefs/releases), and pick the plugin version corresponding to which version of Godot Engine you're using(2.1, or 3.x).
* Download Godot Engine's source code as a zip or tar.gz archive from Godot Engine's [stable releases page](https://github.com/godotengine/godot/releases). Extract it, and copy the "GodotOldPrefs" folder to "modules/".
* To build Godot Engine 2.1 or 3.x, follow these instructions from Godot Engine's documentation: for [Windows](https://github.com/godotengine/godot-docs/blob/master/development/compiling/compiling_for_windows.rst), [Linux](https://github.com/godotengine/godot-docs/blob/master/development/compiling/compiling_for_linuxbsd.rst) and [Mac](https://github.com/godotengine/godot-docs/blob/master/development/compiling/compiling_for_osx.rst).


## License

>The MIT License (MIT)
>
>Copyright 2016-2021 Egor Yakovlev, PigelPix Games, github.com/pigelpixgames (@pigelpixgames)
>
>Permission is hereby granted, free of charge, to any person obtaining a copy
>of this software and associated documentation files (the "Software"), to deal
>in the Software without restriction, including without limitation the rights
>to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
>copies of the Software, and to permit persons to whom the Software is
>furnished to do so, subject to the following conditions:
>
>The above copyright notice and this permission notice shall be included in all
>copies or substantial portions of the Software.
>
>THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
>IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
>FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
>AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
>LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
>OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
>SOFTWARE.
