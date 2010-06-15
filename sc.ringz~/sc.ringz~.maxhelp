{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 395.0, 140.0, 359.0, 535.0 ],
		"bglocked" : 0,
		"defrect" : [ 395.0, 140.0, 359.0, 535.0 ],
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
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"patching_rect" : [ 165.0, 165.0, 50.0, 19.0 ],
					"id" : "obj-14",
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Verdana",
					"numinlets" : 1,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"patching_rect" : [ 80.0, 110.0, 50.0, 19.0 ],
					"id" : "obj-11",
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Verdana",
					"numinlets" : 1,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sig~ 0.3",
					"numoutlets" : 1,
					"patching_rect" : [ 145.0, 220.0, 53.0, 19.0 ],
					"id" : "obj-8",
					"outlettype" : [ "signal" ],
					"fontname" : "Verdana",
					"numinlets" : 1,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sig~ 4000",
					"numoutlets" : 1,
					"patching_rect" : [ 70.0, 150.0, 62.0, 19.0 ],
					"id" : "obj-5",
					"outlettype" : [ "signal" ],
					"fontname" : "Verdana",
					"numinlets" : 1,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "arg1: frequency\narg2: decayTime",
					"linecount" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 140.0, 285.0, 150.0, 31.0 ],
					"id" : "obj-13",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "A 2 pole resonance filter. Bandwith is specified in a 60dB ring decay time.",
					"linecount" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 10.0, 50.0, 291.0, 31.0 ],
					"id" : "obj-6",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sc.dust~ 10",
					"numoutlets" : 1,
					"patching_rect" : [ 10.0, 225.0, 71.0, 19.0 ],
					"id" : "obj-1",
					"outlettype" : [ "signal" ],
					"fontname" : "Verdana",
					"numinlets" : 1,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sc.ringz~ 500 1.2",
					"numoutlets" : 1,
					"patching_rect" : [ 10.0, 291.0, 100.0, 19.0 ],
					"id" : "obj-3",
					"outlettype" : [ "signal" ],
					"fontname" : "Verdana",
					"numinlets" : 3,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 0.4",
					"numoutlets" : 1,
					"patching_rect" : [ 10.0, 360.0, 45.0, 19.0 ],
					"id" : "obj-2",
					"outlettype" : [ "signal" ],
					"fontname" : "Verdana",
					"numinlets" : 2,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "part of sc-max. http://github.com/sbl/sc-max\nport by stephen lumenta\nsupercollider is licensed under the GPL so is this.",
					"linecount" : 3,
					"numoutlets" : 0,
					"patching_rect" : [ 5.0, 458.0, 284.0, 43.0 ],
					"id" : "obj-12",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sc.ringz~",
					"numoutlets" : 0,
					"patching_rect" : [ 11.0, 11.0, 136.0, 24.0 ],
					"id" : "obj-7",
					"fontname" : "Verdana",
					"fontface" : 3,
					"numinlets" : 1,
					"fontsize" : 14.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"numoutlets" : 0,
					"patching_rect" : [ 10.0, 410.0, 45.0, 45.0 ],
					"id" : "obj-4",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"patching_rect" : [ 9.0, 6.0, 134.0, 34.0 ],
					"id" : "obj-10",
					"rounded" : 0,
					"bgcolor" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-3", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-3", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-2", 0 ],
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
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-4", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
