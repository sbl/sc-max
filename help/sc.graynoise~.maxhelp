{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 171.0, 138.0, 864.0, 552.0 ],
		"bglocked" : 0,
		"defrect" : [ 171.0, 138.0, 864.0, 552.0 ],
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
					"id" : "obj-1",
					"patching_rect" : [ 113.0, 214.0, 412.0, 148.0 ],
					"numinlets" : 2,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sc.graynoise~",
					"fontname" : "Verdana",
					"outlettype" : [ "signal" ],
					"id" : "obj-3",
					"fontsize" : 10.0,
					"patching_rect" : [ 23.0, 158.0, 81.0, 19.0 ],
					"numinlets" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 0.2",
					"fontname" : "Verdana",
					"outlettype" : [ "signal" ],
					"id" : "obj-2",
					"fontsize" : 10.0,
					"patching_rect" : [ 23.0, 245.0, 45.0, 19.0 ],
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "part of sc-max. http://github.com/sbl/sc-max\nport by stephen lumenta\nsupercollider is licensed under the GPL so is this.",
					"linecount" : 3,
					"fontname" : "Verdana",
					"id" : "obj-12",
					"fontsize" : 10.0,
					"patching_rect" : [ 5.0, 458.0, 279.0, 43.0 ],
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Generates noise which results from flipping random bits in a word.\nThis type of noise has a high RMS level relative to its peak to peak level.\nThe spectrum is emphasized towards lower frequencies.",
					"linecount" : 3,
					"fontname" : "Verdana",
					"id" : "obj-9",
					"fontsize" : 10.0,
					"patching_rect" : [ 9.0, 42.0, 384.0, 43.0 ],
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sc.graynoise~",
					"fontname" : "Arial",
					"id" : "obj-7",
					"fontface" : 3,
					"fontsize" : 16.0,
					"patching_rect" : [ 9.0, 11.0, 118.0, 25.0 ],
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"id" : "obj-4",
					"patching_rect" : [ 23.0, 316.0, 45.0, 45.0 ],
					"numinlets" : 2,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"id" : "obj-10",
					"patching_rect" : [ 9.0, 6.0, 134.0, 34.0 ],
					"bgcolor" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
					"numinlets" : 1,
					"rounded" : 0,
					"numoutlets" : 0
				}

			}
 ],
		"lines" : [ 			{
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
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
