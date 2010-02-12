{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 230.0, 188.0, 864.0, 552.0 ],
		"bglocked" : 0,
		"defrect" : [ 230.0, 188.0, 864.0, 552.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 10.0,
		"default_fontface" : 0,
		"default_fontname" : "Verdana",
		"gridonopen" : 0,
		"gridsize" : [ 5.0, 5.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess 10",
					"fontsize" : 10.0,
					"numoutlets" : 1,
					"id" : "obj-17",
					"outlettype" : [ "" ],
					"fontname" : "Verdana",
					"patching_rect" : [ 9.0, 94.0, 74.0, 19.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "freq changes waits until one \"phase\" has finished. So very low freq values might seem broken or appear to keep on forever.",
					"linecount" : 2,
					"fontsize" : 10.0,
					"numoutlets" : 0,
					"id" : "obj-5",
					"fontname" : "Verdana",
					"patching_rect" : [ 65.0, 117.0, 330.0, 31.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 10.0,
					"numoutlets" : 2,
					"id" : "obj-16",
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Verdana",
					"patching_rect" : [ 9.0, 124.0, 50.0, 19.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "cycle~",
					"fontsize" : 10.0,
					"numoutlets" : 1,
					"id" : "obj-13",
					"outlettype" : [ "signal" ],
					"fontname" : "Verdana",
					"patching_rect" : [ 9.0, 265.0, 44.0, 19.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "+~ 1000",
					"fontsize" : 10.0,
					"numoutlets" : 1,
					"id" : "obj-11",
					"outlettype" : [ "signal" ],
					"fontname" : "Verdana",
					"patching_rect" : [ 9.0, 229.0, 57.0, 19.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 100",
					"fontsize" : 10.0,
					"numoutlets" : 1,
					"id" : "obj-8",
					"outlettype" : [ "signal" ],
					"fontname" : "Verdana",
					"patching_rect" : [ 9.0, 194.0, 48.0, 19.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Quadratic Noise\n\nGenerates quadratically interpolated random values at a rate.\n\n",
					"linecount" : 3,
					"fontsize" : 10.0,
					"numoutlets" : 0,
					"id" : "obj-6",
					"fontname" : "Verdana",
					"patching_rect" : [ 10.0, 44.0, 343.0, 43.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "scope~",
					"numoutlets" : 0,
					"id" : "obj-1",
					"patching_rect" : [ 350.0, 273.0, 317.0, 161.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sc.lfnoise2~ 10",
					"fontsize" : 10.0,
					"numoutlets" : 1,
					"id" : "obj-3",
					"outlettype" : [ "signal" ],
					"fontname" : "Verdana",
					"patching_rect" : [ 8.0, 153.0, 88.0, 19.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 0.2",
					"fontsize" : 10.0,
					"numoutlets" : 1,
					"id" : "obj-2",
					"outlettype" : [ "signal" ],
					"fontname" : "Verdana",
					"patching_rect" : [ 9.0, 359.0, 45.0, 19.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "part of sc-max. http://github.com/sbl/sc-max\nport by stephen lumenta\nsupercollider is licensed under the GPL so is this.",
					"linecount" : 3,
					"fontsize" : 10.0,
					"numoutlets" : 0,
					"id" : "obj-12",
					"fontname" : "Verdana",
					"patching_rect" : [ 4.0, 471.0, 279.0, 43.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sc.lfnoise2~",
					"fontsize" : 16.0,
					"numoutlets" : 0,
					"id" : "obj-7",
					"fontname" : "Arial",
					"patching_rect" : [ 9.0, 11.0, 125.0, 25.0 ],
					"fontface" : 3,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"numoutlets" : 0,
					"id" : "obj-4",
					"patching_rect" : [ 9.0, 407.0, 45.0, 45.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"rounded" : 0,
					"numoutlets" : 0,
					"id" : "obj-10",
					"bgcolor" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
					"patching_rect" : [ 9.0, 6.0, 134.0, 34.0 ],
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [ 17.5, 177.0, 54.0, 177.0, 97.0, 224.0, 124.0, 243.0, 147.0, 243.0, 166.0, 243.0, 185.0, 259.0, 118.0, 316.0, 128.0, 309.0, 121.0, 325.0, 121.0, 332.0, 149.0, 332.0, 168.0, 313.0, 181.0, 287.0, 219.0, 260.0, 230.0, 260.0, 230.0, 284.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-4", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
