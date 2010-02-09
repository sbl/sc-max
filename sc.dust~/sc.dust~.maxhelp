{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 337.0, 204.0, 640.0, 506.0 ],
		"bglocked" : 0,
		"defrect" : [ 337.0, 204.0, 640.0, 506.0 ],
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
					"maxclass" : "comment",
					"text" : "part of sc-max. http://github.com/sbl/sc-max\nport by stephen lumenta\nsupercollider is licensed under the GPL so is this.",
					"linecount" : 3,
					"numinlets" : 1,
					"id" : "obj-12",
					"patching_rect" : [ 5.0, 458.0, 277.0, 43.0 ],
					"numoutlets" : 0,
					"fontname" : "Verdana",
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "description of the product",
					"numinlets" : 1,
					"id" : "obj-9",
					"patching_rect" : [ 13.0, 38.0, 150.0, 19.0 ],
					"numoutlets" : 0,
					"fontname" : "Verdana",
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Product",
					"numinlets" : 1,
					"id" : "obj-7",
					"patching_rect" : [ 13.0, 11.0, 150.0, 25.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial",
					"fontface" : 3,
					"fontsize" : 16.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numinlets" : 1,
					"id" : "obj-6",
					"patching_rect" : [ 235.0, 140.0, 50.0, 19.0 ],
					"numoutlets" : 2,
					"fontname" : "Verdana",
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"numinlets" : 2,
					"id" : "obj-4",
					"patching_rect" : [ 180.0, 255.0, 45.0, 45.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"sig" : 0.0,
					"mode" : 2,
					"numinlets" : 2,
					"id" : "obj-3",
					"patching_rect" : [ 175.0, 215.0, 56.0, 19.0 ],
					"numoutlets" : 2,
					"fontname" : "Verdana",
					"outlettype" : [ "signal", "float" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"sig" : 0.0,
					"mode" : 1,
					"numinlets" : 2,
					"id" : "obj-2",
					"patching_rect" : [ 175.0, 140.0, 56.0, 19.0 ],
					"numoutlets" : 2,
					"fontname" : "Verdana",
					"outlettype" : [ "signal", "float" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "simplemsp~",
					"numinlets" : 1,
					"id" : "obj-1",
					"patching_rect" : [ 175.0, 175.0, 72.0, 19.0 ],
					"numoutlets" : 1,
					"fontname" : "Verdana",
					"outlettype" : [ "" ],
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"id" : "obj-10",
					"patching_rect" : [ 9.0, 6.0, 162.0, 58.0 ],
					"rounded" : 0,
					"numoutlets" : 0,
					"bgcolor" : [ 0.501961, 0.717647, 0.764706, 1.0 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-1", 0 ],
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
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
