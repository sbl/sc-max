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
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"id" : "obj-1",
					"patching_rect" : [ 339.0, 218.0, 384.0, 270.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sc.whitenoise~",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"id" : "obj-3",
					"fontname" : "Verdana",
					"patching_rect" : [ 249.0, 162.0, 87.0, 19.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 0.2",
					"fontsize" : 10.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"id" : "obj-2",
					"fontname" : "Verdana",
					"patching_rect" : [ 249.0, 249.0, 45.0, 19.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "part of sc-max. http://github.com/sbl/sc-max\nport by stephen lumenta\nsupercollider is licensed under the GPL so is this.",
					"linecount" : 3,
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-12",
					"fontname" : "Verdana",
					"patching_rect" : [ 5.0, 458.0, 279.0, 43.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Generates noise whose spectrum has equal power at all frequencies.",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-9",
					"fontname" : "Verdana",
					"patching_rect" : [ 9.0, 42.0, 357.0, 19.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sc.whitenoise~",
					"fontface" : 3,
					"fontsize" : 16.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-7",
					"fontname" : "Arial",
					"patching_rect" : [ 9.0, 11.0, 125.0, 25.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"id" : "obj-4",
					"patching_rect" : [ 249.0, 320.0, 45.0, 45.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"rounded" : 0,
					"numinlets" : 1,
					"bgcolor" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
					"numoutlets" : 0,
					"id" : "obj-10",
					"patching_rect" : [ 9.0, 6.0, 134.0, 34.0 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-1", 0 ],
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
 ]
	}

}
